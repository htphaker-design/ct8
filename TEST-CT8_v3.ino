//AGV CT8 v6 26/10/2026
#include "control_step.h"
#include "sensor.h"
const int pin_bao_mat_line = 45;
const int pin_dung = 46;// chân returm hàng  
const int time_sau_khi_quet_vach_14lan2 = 5000;     
const unsigned long time_bao_mat_line_lau = 300000; 
const unsigned long time_cho_vach_2 = 8000;        
const unsigned long time_dung_lay_hang = 20000;    
const unsigned long thoi_gian_bo_qua_mat_line = 500; 
const unsigned long thoi_gian_duy_tri_case_9 = 3000;   // Lỡ out line ở case 9 / -9: giữ lái + chạy tiếp thêm 3s rồi mới dừng
const unsigned long thoi_gian_xac_nhan_co_line = 50;   // Thấy lại line liên tục 50ms mới coi là tìm lại được (lọc nhiễu)

const int speed = 1170;
const int slowSpeed = 12000;
const int startSpeed = 12000;
const unsigned long thoi_gian_tang_toc = 9000;
const unsigned long thoi_gian_giam_toc = 6000;

// --- PID GIỐNG AGV SHIZAI ---
const float KP_TBL[10] = {   0,  25,  45,  70,  95, 130, 180, 220, 310, 500 };
const float KD_TBL[10] = { 500, 500, 500, 500, 500, 500, 500, 500, 500, 500 };   // Kd cố định 500 như Shizai
const int   BU_TBL[10] = {   5,  10,  20,  50,  80, 100, 240, 320, 410, 500 };
const float Ki = 0.01;
const float TIME_VAO_CUA = 200.0;
const float TIME_RA_CUA  = 300.0;

int raw_sensor = 0;
int steering_error = 0;
int last_valid_sensor = 0;
bool is_special_code = false;
bool dang_gap_su_co = false;
unsigned long t_bat_dau_su_co = 0;
bool dang_mat_line = false;                 // đang trong cửa sổ mất line (giữ lái)
bool dang_thay_line = false;                // đang đếm xác nhận đã thấy lại line
unsigned long t_bat_dau_mat_line = 0, t_bat_dau_thay_line = 0;
unsigned long cho_phep_chot = 500;          // thời gian cho phép mất line, CHỐT 1 lần lúc bắt đầu mất line
float toc_do_luc_mat_line = 0;              // tốc độ lúc bắt đầu mất line (không cho nhanh hơn khi chạy mù)
bool xe_dang_bi_khoa = false; 
unsigned long thoi_gian_cap_nguon = 0; 
unsigned long thoi_gian_roi_tram = 0;
int trang_thai_tram = 0; 
unsigned long t_gap_vach_1 = 0;
unsigned long t_bat_dau_xa_hang = 0;
unsigned long t_bat_dau_dung_20s = 0;

float currentSpeedVal = startSpeed, smoothBaseSpeed = startSpeed;
float toc_do_khi_bat_dau_giam = startSpeed, toc_do_khi_bat_dau_tang = startSpeed;
unsigned long thoi_gian_bat_dau = 0, thoi_gian_bat_dau_giam = 0, last_ramp_time = 0;
bool dang_tang_toc = true, dang_giam_toc = false;
float smoothedError = 0, lastError = 0, integral = 0, lastPidOutput = 0;
float current_Kp = KP_TBL[0], current_Kd = KD_TBL[0], current_offset = 0.0;

void setup() {
    pinMode(pin_dung, OUTPUT); digitalWrite(pin_dung, HIGH); 
    pinMode(pin_bao_mat_line, OUTPUT); digitalWrite(pin_bao_mat_line, HIGH);
    
    pinMode(PUL_TRAI, OUTPUT); pinMode(DIR_TRAI, OUTPUT); pinMode(ENA_TRAI, OUTPUT);
    pinMode(PUL_PHAI, OUTPUT); pinMode(DIR_PHAI, OUTPUT); pinMode(ENA_PHAI, OUTPUT);
    
    pinMode(T4, INPUT); pinMode(T3, INPUT); pinMode(T2, INPUT); pinMode(T1, INPUT);
    pinMode(TG, INPUT); pinMode(PG, INPUT);
    pinMode(P1, INPUT); pinMode(P2, INPUT); pinMode(P3, INPUT); pinMode(P4, INPUT);
    pinMode(cb1, INPUT_PULLUP); 
    
    digitalWrite(ENA_TRAI, LOW); digitalWrite(ENA_PHAI, LOW);
    thoi_gian_cap_nguon = millis(); 
    KhoiTaoThongSoDeBa();
}

void loop() 
{
    raw_sensor = In_SenSor();
    
    if (KiemTraAnToan()) return;
    DocVaLocCamBien();
    if (XuLyTram()) return;
    QuyetDinhXuatXung();
}

void KhoiTaoThongSoDeBa() 
{
    currentSpeedVal = (float)startSpeed;
    smoothBaseSpeed = (float)startSpeed;
    toc_do_khi_bat_dau_tang = (float)startSpeed;
    thoi_gian_bat_dau = millis();
    last_ramp_time = millis();
    dang_tang_toc = true; 
    dang_giam_toc = false;
    integral = 0; lastError = 0; smoothedError = 0;
    current_Kp = KP_TBL[0]; current_Kd = KD_TBL[0]; current_offset = 0.0;
    lastPidOutput = 0;
}

bool KiemTraAnToan() {
    // ĐÃ FIX: Nếu đang dừng trạm (2: Xả hàng, 3: Lấy hàng) thì xe TỰ ĐỘNG BỎ QUA VẬT CẢN
    bool dang_lam_nhiem_vu_tram = (trang_thai_tram == 2 || trang_thai_tram == 3);
    
    bool co_vat_can = (digitalRead(cb1) == LOW) && !dang_lam_nhiem_vu_tram; 
    bool mat_line = (raw_sensor == 13); 

    if (co_vat_can) 
    {    
        step_dc(false, false, HIGH, LOW, 0, 0); 
        xe_dang_bi_khoa = true; 
        
        if (!dang_gap_su_co) {
            dang_gap_su_co = true;
            t_bat_dau_su_co = millis(); 
        } else if (millis() - t_bat_dau_su_co >= time_bao_mat_line_lau) 
        {
            digitalWrite(pin_bao_mat_line, LOW); 
        }
        return true; 
    } 
    else if (mat_line) 
    {
        if (!dang_gap_su_co) {
            dang_gap_su_co = true;
            t_bat_dau_su_co = millis(); 
        }
        dang_thay_line = false;                 // đang mất line -> huỷ đếm xác nhận
        if (!dang_mat_line) {                   // vừa mới mất line: CHỐT 1 lần
            dang_mat_line = true;
            t_bat_dau_mat_line = millis();
            toc_do_luc_mat_line = smoothBaseSpeed;
            // Đang chạy bình thường và mã cuối là ±9 -> cho giữ lái 3s, còn lại chỉ 500ms
            cho_phep_chot = (trang_thai_tram == 0 && abs(last_valid_sensor) == 9)
                            ? thoi_gian_duy_tri_case_9 : thoi_gian_bo_qua_mat_line;
        }
        
        if (millis() - t_bat_dau_mat_line < cho_phep_chot) 
        {
            return false; 
        } else {
            step_dc(false, false, HIGH, LOW, 0, 0); 
            xe_dang_bi_khoa = true; 
            
            if (millis() - t_bat_dau_mat_line >= time_bao_mat_line_lau) 
            {
                digitalWrite(pin_bao_mat_line, LOW); 
            }
            return true; 
        }
    } 
    else {
        if (dang_gap_su_co) 
        {
            // Đang trong cửa sổ mất line: phải thấy line liên tục >= 50ms mới coi là tìm lại được
            if (dang_mat_line) {
                if (!dang_thay_line) { dang_thay_line = true; t_bat_dau_thay_line = millis(); }
                if (millis() - t_bat_dau_thay_line < thoi_gian_xac_nhan_co_line) {
                    if (xe_dang_bi_khoa) { step_dc(false, false, HIGH, LOW, 0, 0); return true; }
                    return false;   // vẫn trong cửa sổ giữ: tiếp tục chạy với lái đã khoá
                }
            }
            bool vua_het_mat_line = dang_mat_line;
            dang_gap_su_co = false;
            dang_mat_line = false;
            dang_thay_line = false;
            digitalWrite(pin_bao_mat_line, HIGH); 
            
            if (xe_dang_bi_khoa) 
            {
                KhoiTaoThongSoDeBa(); 
                xe_dang_bi_khoa = false;
            }
            else if (vua_het_mat_line)
            {
                // Lúc chạy mù đồng hồ tăng/giảm tốc vẫn chạy ngầm -> tìm lại line là tốc độ nhảy vọt.
                // Tính lại đà từ tốc độ hiện tại để xe nối tiếp êm.
                unsigned long t_nay = millis();
                if (dang_tang_toc) { toc_do_khi_bat_dau_tang = smoothBaseSpeed; thoi_gian_bat_dau = t_nay; }
                if (dang_giam_toc) { toc_do_khi_bat_dau_giam = smoothBaseSpeed; thoi_gian_bat_dau_giam = t_nay; }
            }
        }
        return false;
    }
}

void DocVaLocCamBien() {
    if (raw_sensor == 14 && trang_thai_tram == 0) {
        if (abs(last_valid_sensor) >= 5 || abs(smoothedError) > 4.5) 
        {
            raw_sensor = (last_valid_sensor > 0) ? 9 : -9;
        }
    }
    is_special_code = (raw_sensor == 12 || raw_sensor == 13 || raw_sensor == 14);
    // Đang trong cửa sổ mất line: khoá lái + last_valid_sensor, bỏ qua nhiễu 1-2 khung hình
    if (dang_mat_line) { steering_error = last_valid_sensor; return; }
    if (raw_sensor >= -9 && raw_sensor <= 9) {      // mã lệch hợp lệ
        steering_error = raw_sensor;
        last_valid_sensor = raw_sensor;
    } else if (raw_sensor == 13) {
        steering_error = last_valid_sensor;
    } else if (raw_sensor == 12 || raw_sensor == 14) {
        steering_error = 0;
    }
    // 15 và 99 (nhiễu / ngã ba): giữ nguyên lái cũ
}

bool XuLyTram() {
    unsigned long now = millis();
    
    // Bỏ qua trạm trong 9s đầu tiên khi mới bật nguồn
    if (now - thoi_gian_cap_nguon < thoi_gian_tang_toc) 
    {
        return false; 
    }
    
    if (trang_thai_tram == 0) 
    {
        // ĐÃ THÊM LỌC: Cách 4 giây từ lúc rời trạm mới cho phép nhận lại vạch 14
        if (raw_sensor == 14 && (now - thoi_gian_roi_tram >= time_sau_khi_quet_vach_14lan2)) 
        {
            trang_thai_tram = 1;
            t_gap_vach_1 = now;
            dang_tang_toc = false;
            dang_giam_toc = true;
            thoi_gian_bat_dau_giam = now;
            toc_do_khi_bat_dau_giam = smoothBaseSpeed;
        }
    }
    else if (trang_thai_tram == 1) {
        if (now - t_gap_vach_1 > 500) {
            if (raw_sensor == 14) {
                trang_thai_tram = 3;
                t_bat_dau_dung_20s = now;
            }
            else if (now - t_gap_vach_1 >= time_cho_vach_2) 
            { 
                trang_thai_tram = 2;
                t_bat_dau_xa_hang = now;
                digitalWrite(pin_dung, LOW); 
            }
        }
    }
    else if (trang_thai_tram == 2) {
        step_dc(false, false, HIGH, LOW, 0, 0); 
        if (now - t_bat_dau_xa_hang >= 500) {
            digitalWrite(pin_dung, HIGH); 
            trang_thai_tram = 4; 
            KhoiTaoThongSoDeBa();
            
            // Xả hàng xong, xe đề ba -> Kích hoạt bịt mắt 4s
            thoi_gian_roi_tram = now; 
        }
        return true; 
    }
    else if (trang_thai_tram == 3) {
        step_dc(false, false, HIGH, LOW, 0, 0); 
        if (now - t_bat_dau_dung_20s >= time_dung_lay_hang) { 
            trang_thai_tram = 4; 
            KhoiTaoThongSoDeBa();
            thoi_gian_roi_tram = now; 
        }
        return true; 
    }
    else if (trang_thai_tram == 4) {
        if (raw_sensor >= -9 && raw_sensor <= 9) {
            trang_thai_tram = 0;
        }
    }
    return false; 
}
void QuyetDinhXuatXung() 
{
    TinhToanBiendangTocDo();
    TinhToanVaXuatPID();
}

void TinhToanBiendangTocDo() 
{
    if (dang_tang_toc) {
        unsigned long t_chay = millis() - thoi_gian_bat_dau;
        if (t_chay < thoi_gian_tang_toc) {
            // nội suy theo tần số xung (1/chu kỳ): tốc độ thật của xe tăng đều theo thời gian
            currentSpeedVal = 1.0f / ( 1.0f / toc_do_khi_bat_dau_tang
                            + (1.0f / speed - 1.0f / toc_do_khi_bat_dau_tang) * ((float)t_chay / thoi_gian_tang_toc) );
        } else {
            currentSpeedVal = (float)speed; 
            dang_tang_toc = false;
        }
    }
    else if (dang_giam_toc) {
        unsigned long t_giam = millis() - thoi_gian_bat_dau_giam;
        if (t_giam < thoi_gian_giam_toc) {
            // Giống Shizai: nội suy tuyến tính theo chu kỳ -> phanh mạnh ngay khi gặp vạch 14 lần 1
            currentSpeedVal = toc_do_khi_bat_dau_giam + ((slowSpeed - toc_do_khi_bat_dau_giam) * ((float)t_giam / thoi_gian_giam_toc));
        } else {
            currentSpeedVal = (float)slowSpeed; 
            dang_giam_toc = false;
        }
    }
    smoothBaseSpeed = (currentSpeedVal * 0.1) + (smoothBaseSpeed * 0.9);
    // Đang mất line (chạy mù): không cho nhanh hơn tốc độ lúc mất line (giá trị nhỏ = nhanh)
    if (dang_mat_line && smoothBaseSpeed < toc_do_luc_mat_line) smoothBaseSpeed = toc_do_luc_mat_line;
}

void TinhToanVaXuatPID() {
    int baseSpeed = (int)smoothBaseSpeed;
    smoothedError = ((float)steering_error * 0.35) + (smoothedError * 0.65); 
    int absError = abs(steering_error);
    int idx = (absError > 9) ? 9 : absError;
    float target_Kp = KP_TBL[idx];
    float target_Kd = KD_TBL[idx];
    int target_offset_raw = BU_TBL[idx];
    float speed_ratio = (float)(startSpeed - baseSpeed) / (startSpeed - speed);
    speed_ratio = constrain(speed_ratio, 0.0, 1.0);
    int target_offset = (int)(target_offset_raw * speed_ratio);
    int UPDATE_RATE = 5;
    if (millis() - last_ramp_time >= UPDATE_RATE) {
        last_ramp_time = millis();
        float time_vao = max(TIME_VAO_CUA, 1.0f);
        float time_ra  = max(TIME_RA_CUA,  1.0f);
        float step_offset_vao = ((float)BU_TBL[9] / time_vao) * UPDATE_RATE;
        float step_offset_ra  = ((float)BU_TBL[9] / time_ra) * UPDATE_RATE;
        float step_Kp_vao = ((KP_TBL[9] - KP_TBL[0]) / time_vao) * UPDATE_RATE;
        float step_Kp_ra  = ((KP_TBL[9] - KP_TBL[0]) / time_ra) * UPDATE_RATE;
        float step_Kd_vao = ((KD_TBL[9] - KD_TBL[0]) / time_vao) * UPDATE_RATE;
        float step_Kd_ra  = ((KD_TBL[9] - KD_TBL[0]) / time_ra) * UPDATE_RATE;   
        if (current_offset < target_offset) current_offset = min(current_offset + step_offset_vao, (float)target_offset);
        else if (current_offset > target_offset) current_offset = max(current_offset - step_offset_ra, (float)target_offset);
        if (current_Kp < target_Kp) current_Kp = min(current_Kp + step_Kp_vao, target_Kp);
        else if (current_Kp > target_Kp) current_Kp = max(current_Kp - step_Kp_ra, target_Kp);
        if (current_Kd < target_Kd) current_Kd = min(current_Kd + step_Kd_vao, target_Kd);
        else if (current_Kd > target_Kd) current_Kd = max(current_Kd - step_Kd_ra, target_Kd);
    }
    int finalBaseSpeed = baseSpeed + (int)current_offset;
    finalBaseSpeed = constrain(finalBaseSpeed, speed, startSpeed);
    integral = constrain(integral + smoothedError, -100, 100);
    float derivative = smoothedError - lastError;
    derivative = constrain(derivative, -2.0, 2.0); 
    float rawPid = (current_Kp * smoothedError) + (Ki * integral) + (current_Kd * derivative);
    lastError = smoothedError;
    float filteredPid = (rawPid * 0.05) + (lastPidOutput * 0.95);
    if (filteredPid > lastPidOutput + 100) filteredPid = lastPidOutput + 100;
    else if (filteredPid < lastPidOutput - 100) filteredPid = lastPidOutput - 100;
    lastPidOutput = filteredPid;
    int PHUC_DEPTRAI = constrain(finalBaseSpeed - (int)filteredPid, speed, startSpeed);
    int PHUC_KDEP = constrain(finalBaseSpeed + (int)filteredPid, speed, startSpeed);
    step_dc(true, true, HIGH, LOW, PHUC_KDEP, PHUC_DEPTRAI);
}
