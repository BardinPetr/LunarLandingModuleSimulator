#include <SoftwareSerial.h>
#include <SBGC_Arduino.h>
#include <inttypes.h>
#include <SBGC.h>

#define SBGC_CMD_DELAY 20

SoftwareSerial serial(6, 7);
SBGC_cmd_control_t cc = { 0, 0, 0, 0, 0, 0, 0 };

static uint16_t cur_time_ms, low_rate_time_ms, last_cmd_time_ms, rt_req_last_time_ms, joy_last_time_ms;
static SBGC_cmd_realtime_data_t rt_data;
static uint8_t cur_adj_var_idx = 0;
static SBGC_cmd_control_ext_t cmd_control = {
  { SBGC_CONTROL_MODE_ANGLE, SBGC_CONTROL_MODE_RC, SBGC_CONTROL_MODE_RC },
  { { 0, 0 }, { 0, 0 }, { 0, 0 } }
};
bool is_connected = 0;

int speedstate = 8;
int iter = 0;

void set_spd(int id, int _speed);
int units_to_degrees(int x);
void fix_rtdata();
void send_data();
void process();
void request();

void gimbal_setup() {
  serial.begin(115200);
  SBGC_Demo_setup(&serial);
}

void gimbal_run() {
  process();
  request();
}

void request() {
  if ((cur_time_ms - rt_req_last_time_ms) > 30) {
    SerialCommand cmd;
    if (is_connected) {
      cmd.init(SBGC_CMD_REALTIME_DATA);
    } else {
      cmd.init(SBGC_CMD_BOARD_INFO);
    }
    sbgc_parser.send_cmd(cmd, 0);
    rt_req_last_time_ms = cur_time_ms;
  }
}

void set_connected() {
  is_connected = 1;
}

void process() {
  while (sbgc_parser.read_cmd()) {
    if (!is_connected) set_connected();

    SerialCommand &cmd = sbgc_parser.in_cmd;
    last_cmd_time_ms = cur_time_ms;
    uint8_t error = 0;
    switch (cmd.id) {
      case SBGC_CMD_REALTIME_DATA:
      case SBGC_CMD_REALTIME_DATA_3:
      case SBGC_CMD_REALTIME_DATA_4:
        error = SBGC_cmd_realtime_data_unpack(rt_data, cmd);
        fix_rtdata();
        send_data();
        break;
    }
  }
}

void send_data() {
  String s = "";
  s += String(rt_data.imu_angle[ROLL]) + "@";
  s += String(rt_data.imu_angle[PITCH]) + "@";
  s += String(rt_data.imu_angle[YAW]) + "\r";
  Serial.print(s);
}

void fix_rtdata() {
  rt_data.imu_angle[PITCH] = units_to_degrees(rt_data.imu_angle[PITCH]);
  rt_data.imu_angle[ROLL] = units_to_degrees(rt_data.imu_angle[ROLL]);
  rt_data.imu_angle[YAW] = units_to_degrees(rt_data.imu_angle[YAW]);
}

int units_to_degrees(int x) {
  return (double)(x) * 0.106;
}

void set_spd(int id, int _speed) {
  _speed *= SBGC_SPEED_SCALE;
  SBGC_cmd_control_t c = { 0, 0, 0, 0, 0, 0, 0 };

  c.mode = SBGC_CONTROL_MODE_SPEED;
  c.speedROLL = (id == ROLL ? _speed : 0);
  c.speedPITCH = (id == PITCH ? _speed : 0);
  c.speedYAW = (id == YAW ? _speed : 0);
  SBGC_cmd_control_send(c, sbgc_parser);
}

void run_spd(int id, bool R) {
  if (R) {
    if ((iter % 20) == 0) {
      speedstate += (speedstate + 1 < 25 ? 1 : 0);
    }
  }
  else {
    if ((iter % 20) == 0) {
      speedstate -= (speedstate - 1 > 25 ? 1 : 0);
    }
  }
  set_spd(id, speedstate);
  iter ++;
}
