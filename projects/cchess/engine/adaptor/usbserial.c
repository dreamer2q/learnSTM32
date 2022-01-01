#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <time.h>

// 发送超时
const int TIMEOUT = 2;  // seconds
// 串口目标
const char* SERIAL_PORT = "/dev/tty.usbserial-CBAOb10CD20";
// 输出日志
const char* LOG_FILE = "serial.log";
// 引擎位置
const char* ENGINE_PATH = "../build/eleeye";

/**
 * 打开一个串口
 */
int open_serial(const char* serialport) {
  // O_NDELAY 函数使 read 函数在端口没有字符存在的情况下，立刻返回0
  // 与 fcntl(fd, F_SETFL, FNDELAY) 一样
  int fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd < 0) {
    return -1;
  }
  if (isatty(fd) == 0) {
    // not a terminal device
    fprintf(stderr, "%s: not a terminal device\n", serialport);
    return -1;
  }
  return fd;
}

/**
 * 设置串口参数
 */
int set_serial_opt(int fd, int speed, int nbits, char parity, int nstop) {
  struct termios tty = {0};

  if (tcgetattr(fd, &tty) != 0) {
    close(fd);
    return -1;
  }

  tty.c_lflag = 0;  // no signaling chars, no echo,
                    // no canonical processing
  tty.c_oflag = 0;  // no remapping, no delays
  tty.c_iflag = 0;

  // input modes
  tty.c_iflag |= IGNCR;  // ignore CR

  // tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // shut off xon/xoff ctrl
  // disable IGNBRK for mismatched speed tests;
  // otherwise receive break as \000 chars
  // tty.c_iflag &= ~IGNBRK;  // disable break processing

  // output modes
  tty.c_oflag |= OPOST;  // Post-process output

  tty.c_cc[VMIN] = 0;   // read doesn't block
  tty.c_cc[VTIME] = 0;  // ..immediately from read()

  // control modes
  tty.c_cflag |= (CLOCAL | CREAD);  // ignore modem controls,
                                    // enable reading
                                    // tty.c_cflag &= ~CRTSCTS;

  // local modes
  tty.c_lflag |= IEXTEN;  // Extended input character processing

  // Special characters
  tty.c_cc[VMIN] = 0;   // Always return...
  tty.c_cc[VTIME] = 0;  // ..immediately from read()

  // tty.c_cc[VSTOP] = XOFF;
  // tty.c_cc[VSTART] = XON;

  // 设置数据位
  tty.c_cflag &= ~CSIZE;
  if (nbits == 7) {
    tty.c_cflag |= CS7;
  } else {
    tty.c_cflag |= CS8;
  }

  // 设置停止位数
  // 若停止位为1，则清除CSTOPB，若停止位为2，则激活CSTOPB
  if (nstop == 2) {
    tty.c_cflag |= CSTOPB;
  } else {
    tty.c_cflag &= ~CSTOPB;
  }

  // 设置奇偶校验
  switch (parity) {
    case 'O':
    case 'o':
      tty.c_cflag |= PARENB | PARODD;
      break;
    case 'E':
    case 'e':
      tty.c_cflag |= PARENB;
      tty.c_cflag &= ~PARODD;
      break;
    default:
      tty.c_cflag &= ~PARENB;
      break;
  }

  // 设置波特率
  switch (speed) {
    case B2400:
    case B4800:
    case B9600:
    case B115200:
    case B230400:
      cfsetispeed(&tty, speed);
      cfsetispeed(&tty, speed);
      break;
    default:
      fprintf(stderr, "warning: baud rate %d is not supported, using 9600.\n",
              speed);
      cfsetispeed(&tty, B9600);
      cfsetospeed(&tty, B9600);
  }

  // tcflush 清空终端未完成的输入/输出请求及数据
  // TCIFLUSH 表示清空正收到的数据，且不读取出来
  if (tcflush(fd, TCIFLUSH) != 0) {
    perror("tcflush");  // warning
  }

  if ((tcsetattr(fd, TCSANOW, &tty)) != 0) {
    close(fd);
    return -1;
  }
  return 0;
}

int serial_has_char(int fd) {
  struct pollfd fds = {
      .fd = fd,
      .events = POLLIN | POLLPRI,
  };
  return poll(&fds, 1, 0) > 0;
}

void serial_waitsent(int fd) {
  while (tcdrain(fd) == -1) {
    fprintf(stderr, "warning: could not drain data: %s\n", strerror(errno));
  }
}

int serial_write(int fd, const char* buf, int len) {
  time_t start = time(NULL);
  int sent = 0;

  while ((sent < len) && (difftime(time(NULL), start) < TIMEOUT)) {
    int t = write(fd, (buf + sent), (len - sent));
    if (t == -1) {
      fprintf(stderr, "error: write: %s\n", strerror(errno));
      return sent;
    } else {
      sent += t;
    }
  }

  return sent;
}

int serial_read(int fd, char* buf, int len) {
  time_t start = time(NULL);
  int recv = 0;

  while ((recv < len) && (difftime(time(NULL), start) < TIMEOUT)) {
    int t = read(fd, (buf + recv), (len - recv));
    if (t == -1) {
      fprintf(stderr, "error: read: %s\n", strerror(errno));
      return recv;
    } else {
      recv += t;
    }
  }

  return recv;
}

void serial_putchar(int fd, char c) {
  while (serial_write(fd, &c, 1) != 1) {
    // empty block
  }
}

char serial_getchar(int fd) {
  char ch;
  while (serial_read(fd, &ch, 1) != 1) {
    // empty block
  }
  return ch;
}

void serial_puts(int fd, const char* s) {
  while (*s) {
    serial_putchar(fd, *(s++));
  }
}

void serial_getline(int fd, char* buf) {
  char ch;
  while ((ch = serial_getchar(fd)) != '\n') {
    *(buf++) = ch;
  }
  *buf = '\0';
}

int serial_close(int fd) {
  tcflush(fd, TCIOFLUSH);
  return close(fd);
}

/**
 * 检查 fd 是否有数据
 */
int has_input(int fd) {
  struct pollfd fds = {
      .fd = fd,
      .events = POLLIN | POLLPRI,
  };
  return poll(&fds, 1, 0) > 0;
}

int kbhit() {
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);  // STDIN_FILENO is 0
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

enum block {
  DISABLE,
  ENABLE,
};

void nonblock(enum block state) {
  struct termios ttystate;

  // get the terminal state
  tcgetattr(STDIN_FILENO, &ttystate);

  if (state == ENABLE) {
    // turn off canonical mode
    ttystate.c_lflag &= ~ICANON;
    // ttystate.c_lflag &= ~ECHO;
    // minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
  } else if (state == DISABLE) {
    // turn on canonical mode
    ttystate.c_lflag |= ICANON;
    // ttystate.c_lflag |= ECHO;
  }
  // set the terminal attributes.
  tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

void doexit() { nonblock(DISABLE); }

int main(int argc, char* argv[]) {
  int fd = open_serial(SERIAL_PORT);
  if (fd < 0) {
    perror("open serial");
    return -1;
  }

  if (set_serial_opt(fd, 9600, 8, 'N', 1) < 0) {
    perror("serial option");
    return -1;
  }

  int logfd = open("serial.log", O_CREAT | O_RDWR | O_APPEND, 0644);
  if (logfd < 0) {
    perror("open log file");
    return -1;
  }

  fprintf(stderr, "connected to %s\n", SERIAL_PORT);

  atexit(doexit);

  /**
   * STDOUT 无缓冲
   */
  setvbuf(stdout, NULL, _IONBF, 0);

  /**
   * 匿名管道，用于重定向输入输出
   */
  int pipe_stdin[2], pipe_stdout[2];
  if (pipe(pipe_stdin) < 0) {
    perror("pipe stdin");
    return -1;
  }
  if (pipe(pipe_stdout) < 0) {
    perror("pipe stdout");
    return -1;
  }

  pid_t pid;
  if ((pid = fork()) < 0) {
    perror("fork");
    return -1;
  }

  if (pid == 0) {
    // engine process

    /**
     * 重定向输入输出
     */
    close(pipe_stdin[1]);   // write
    close(pipe_stdout[0]);  // read
    dup2(pipe_stdin[0], STDIN_FILENO);
    dup2(pipe_stdout[1], STDOUT_FILENO);
    close(pipe_stdin[0]);
    close(pipe_stdout[1]);

    /**
     * 打开引擎
     */
    if (execv(ENGINE_PATH, NULL) < 0) {
      fprintf(stderr, "error: exec %s: %s\n", ENGINE_PATH, strerror(errno));
    }
    exit(-1);
  }

  int ein = pipe_stdout[0];
  int eout = pipe_stdin[1];

  nonblock(ENABLE);
  for (;;) {
    char serialch;
    char keych;
    char ech;

    // 键盘输入
    if (kbhit()) {
      // keych = fgetc(stdin);
      read(STDIN_FILENO, &keych, 1);
      serial_putchar(fd, keych);
    }

    // 串口输入
    if (serial_has_char(fd)) {
      serialch = serial_getchar(fd);
      // putchar(serialch);
      // fputc(serialch, stdout);
      write(STDOUT_FILENO, &serialch, 1);
      write(eout, &serialch, 1);
      write(logfd, &serialch, 1);
    }

    // Engine 输入
    if (has_input(ein)) {
      read(ein, &ech, 1);
      write(STDOUT_FILENO, &ech, 1);
      write(logfd, &ech, 1);
      serial_putchar(fd, ech);
    }
  }
  nonblock(DISABLE);

  serial_close(fd);
  close(eout);
  close(ein);
  close(logfd);
}
