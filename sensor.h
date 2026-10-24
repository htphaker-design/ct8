//AGV CT8 08-09-2026
#ifndef SENSOR_H
#define SENSOR_H
const int TG = 31;      const int PG = 30;
const int T1 = 32;      const int P1 = 29;
const int T2 = 33;      const int P2 = 28;
const int T3 = 34;      const int P3 = 27;
const int T4 = 35;      const int P4 = 26;
const int cb1 = 25; 
int g_ma_truoc = 0;
int DocMauCamBien()
{
  int ss1  = digitalRead(T4);
  int ss2  = digitalRead(T3);
  int ss3  = digitalRead(T2);
  int ss4  = digitalRead(T1);
  int ss5  = digitalRead(TG);
  
  int ss6  = digitalRead(PG);
  int ss7  = digitalRead(P1);
  int ss8  = digitalRead(P2);
  int ss9  = digitalRead(P3);
  int ss10 = digitalRead(P4);

  // --- CASE 0: GIỮA LINE ---
  if(ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1) {return 0;}

  // --- LỆCH PHẢI (Giá trị dương) ---
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 1;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 0 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 2;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 0 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 3;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 0 && ss8 == 0 && ss9 == 1 && ss10 == 1)){return 4;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 0 && ss9 == 1 && ss10 == 1)){return 5;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 0 && ss9 == 0 && ss10 == 1)){return 6;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 0 && ss10 == 1)){return 7;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 0 && ss10 == 0)){return 8;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 0)){return 9;}

  // --- LỆCH TRÁI (Giá trị âm) ---
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -1;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 0     && ss5 == 0 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -2;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 0     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -3;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 0 && ss4 == 0     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -4;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 0 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -5;}
  else if((ss1 == 1 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -6;}
  else if((ss1 == 1 && ss2 == 0 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -7;}
  else if((ss1 == 0 && ss2 == 0 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -8;}
  else if((ss1 == 0 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -9;}
  //CASE 14: VẠCH TRẠM/DỪNG
  else if((ss1 == 1 && ss2 == 1 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 0 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)) 
          {return 14;}
  //CASE 15: TRẠM MÓC HÀNG
  else if((ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 0 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)) 
          {return 15;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 13;}
  else if((ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)){return 12;}

  // --- KHÔNG KHỚP MẪU NÀO: nhiễu / ngã ba ---
  int v[10] = {ss1, ss2, ss3, ss4, ss5, ss6, ss7, ss8, ss9, ss10};
  int so_doan = 0, tot_dau = -1, tot_cuoi = -1, tot_len = 0, tot_kc = 100;
  for (int i = 0; i < 10; ) {
    if (v[i] == 0) {
      int j = i;
      while (j + 1 < 10 && v[j + 1] == 0) j++;
      int kc = abs((-9 + i + j) - g_ma_truoc);
      so_doan++;
      if (kc < tot_kc) { tot_kc = kc; tot_len = j - i + 1; tot_dau = i; tot_cuoi = j; }
      i = j + 1;
    } else i++;
  }
  if (abs(g_ma_truoc) <= 2) {                 // đang chạy thẳng: ép thẳng, bỏ qua nhánh rẽ
    if (ss5 == 0 && ss6 == 0) return 0;
    if (ss5 == 0) return (g_ma_truoc == 0) ? 0 : -1;
    if (ss6 == 0) return (g_ma_truoc == 0) ? 0 :  1;
    if (so_doan >= 2) { if (tot_len <= 2 && tot_kc <= 2) return -9 + tot_dau + tot_cuoi; return 99; }
    if (tot_len <= 3 && tot_kc <= 2) return -9 + tot_dau + tot_cuoi;
    return 99;
  }
  // đang ôm cua
  if (so_doan == 1 && tot_len <= 4 && tot_kc <= 4) return -9 + tot_dau + tot_cuoi;
  if (so_doan >= 2 && tot_len <= 2 && tot_kc <= 2) return -9 + tot_dau + tot_cuoi;
  return 99;
}

int In_SenSor()
{
  int ma = DocMauCamBien();
  if (ma >= -9 && ma <= 9) g_ma_truoc = ma;
  return ma;
}
#endif

/*
#ifndef SENSOR_H
#define SENSOR_H

const int TG = 31;      const int PG = 30;
const int T1 = 32;      const int P1 = 29;
const int T2 = 33;      const int P2 = 28;
const int T3 = 34;      const int P3 = 27;
const int T4 = 35;      const int P4 = 26;
const int cb1 = 25; 

int In_SenSor()
{
  int ss1  = digitalRead(T4);
  int ss2  = digitalRead(T3);
  int ss3  = digitalRead(T2);
  int ss4  = digitalRead(T1);
  int ss5  = digitalRead(TG);
  int ss6  = digitalRead(PG);
  int ss7  = digitalRead(P1);
  int ss8  = digitalRead(P2);
  int ss9  = digitalRead(P3);
  int ss10 = digitalRead(P4);

  // ================= LỚP 1: ƯU TIÊN TUYỆT ĐỐI CÁC MÃ ĐẶC BIỆT (LÊN ĐẦU) =================
  // Đặt ở đây để Arduino kiểm tra trạm trước, tránh bị các Case lệch lái ở dưới nuốt chửng.

  // --- CASE 12: QUAY ĐẦU (Toàn bộ mắt thấy đen) ---
  if((ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 0 && ss5 == 0 && ss6 == 0 && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)){return 12;}

  // --- CASE 13: MẤT LINE (Toàn bộ mắt thấy trắng) ---
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1 && ss5 == 1 && ss6 == 1 && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 13;}

  // --- CASE 14: VẠCH TRẠM NẰM BÊN PHẢI (Hoặc bao phủ diện rộng) ---
  else if((ss1 == 1 && ss2 == 1 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 0 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)||
          (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 0 && ss8 == 0 && ss9 == 0 && ss10 == 0)) 
          {return 14;}

  // --- CASE 15: TRẠM MÓC HÀNG BÊN TRÁI (ĐÃ FIX: Giữ tâm bám line 0, mắt trái cạ vạch phụ 0) ---
  // Điều kiện này bắt buộc phải có ss5 hoặc ss6 bằng 0 (đang bám line chính) và ss1, ss2 bằng 0 (chạm vạch phụ trái).
  // Vì các case lệch trái ở dưới bắt buộc ss5, ss6 phải bằng 1, nên Case 15 đặt ở đây biệt lập hoàn toàn, không sợ trùng.
  else if((ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 0     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 0 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
          (ss1 == 0 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)) 
          {return 15;}


  // ================= LỚP 2: CÁC CASE DÒ LINE DÙNG CHO PID CHẠY TỰ ĐỘNG =================

  // --- CASE 0: CHÍNH GIỮA TÂM LINE ---
  if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
     (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)||
     (ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)) {return 0;}


  // --- LỆCH PHẢI (Giá trị dương) ---
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 1;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 0     && ss7 == 0 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 2;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 0 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return 3;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 0 && ss8 == 0 && ss9 == 1 && ss10 == 1)){return 4;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 0 && ss9 == 1 && ss10 == 1)){return 5;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 0 && ss9 == 0 && ss10 == 1)){return 6;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 0 && ss10 == 1)){return 7;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 0 && ss10 == 0)){return 8;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 0)){return 9;}

  // --- LỆCH TRÁI (Giá trị âm) ---
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 0 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -1;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 0     && ss5 == 0 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -2;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 1 && ss4 == 0     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -3;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 0 && ss4 == 0     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -4;}
  else if((ss1 == 1 && ss2 == 1 && ss3 == 0 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -5;}
  else if((ss1 == 1 && ss2 == 0 && ss3 == 0 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -6;}
  else if((ss1 == 1 && ss2 == 0 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -7;}
  else if((ss1 == 0 && ss2 == 0 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -8;}
  else if((ss1 == 0 && ss2 == 1 && ss3 == 1 && ss4 == 1     && ss5 == 1 && ss6 == 1     && ss7 == 1 && ss8 == 1 && ss9 == 1 && ss10 == 1)){return -9;}

  return 13; 
}
#endif*/
