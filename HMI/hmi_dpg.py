import math
import os
import struct
import threading
import time
from collections import deque

import dearpygui.dearpygui as dpg
import serial


# =========================================================
# CONFIG & PIXEL MATH (Tối ưu cho màn hình có Taskbar)
# =========================================================
SERIAL_PORT = "COM9"
BAUD_RATE = 115200

FRAME_HEADER = b"\xAA\x55"
FRAME_SIZE = 26
DEMO_MODE = False

# Full HD 1920x1080
WINDOW_W = 1920
WINDOW_H = 1080

# Căn lề tuyệt đối
MARGIN_X = 20
GAP_X = 20
GAP_Y = 20

# Khai báo chiều cao tiêu đề (Chính là biến bị thiếu)
TITLE_BAR_H = 65
PANEL_HEADER_H = 45

# Hàng trên (Kéo dài TOP_H lên 520 để ô Raw Data không bị cụt)
TOP_LEFT_W = 1280
TOP_RIGHT_W = 580
TOP_H = 520  

# Hàng dưới: tăng chiều cao để tận dụng khung 1080p
BOT_W = 930
BOT_H = 415

MAX_HISTORY = 160


# =========================================================
# FONT HELPER
# =========================================================
def pick_font(*paths):
    for p in paths:
        if os.path.exists(p): return p
    return "C:/Windows/Fonts/segoeui.ttf"

FONT_UI = pick_font("C:/Windows/Fonts/segoeui.ttf", "C:/Windows/Fonts/arial.ttf")
FONT_BOLD = pick_font("C:/Windows/Fonts/segoeuib.ttf", "C:/Windows/Fonts/arialbd.ttf")
FONT_METRIC_MAIN  = pick_font("C:/Windows/Fonts/bahnschrift.ttf",
                              "C:/Windows/Fonts/segoeuib.ttf",
                              "C:/Windows/Fonts/arialbd.ttf")

FONT_METRIC_SUB   = pick_font("C:/Windows/Fonts/bahnschrift.ttf",
                              "C:/Windows/Fonts/segoeui.ttf",
                              "C:/Windows/Fonts/arial.ttf")

FONT_METRIC_VALUE = pick_font("C:/Windows/Fonts/consolab.ttf",
                              "C:/Windows/Fonts/consola.ttf",
                              "C:/Windows/Fonts/arialbd.ttf")

# =========================================================
# FRONT VIEW IMAGE ASSETS
# =========================================================
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
ASSET_DIR = os.path.join(BASE_DIR, "hmi_assets")
ROAD_IMAGE_PATH = os.path.join(ASSET_DIR, "road.png")
CAR_IMAGE_PATH  = os.path.join(ASSET_DIR, "car.png")
GREEN_WARNING_IMAGE_PATH = os.path.join(ASSET_DIR, "green_warning.png")
YELLOW_WARNING_IMAGE_PATH = os.path.join(ASSET_DIR, "yellow_warning.png")
RED_WARNING_IMAGE_PATH    = os.path.join(ASSET_DIR, "red_warning.png")
BSD_WARNING_IMAGE_PATH = os.path.join(ASSET_DIR, "bsd_warning.png")

# =========================================================
# GLOBAL DATA
# =========================================================
running = True
data_lock = threading.Lock()

latest_data = {
    "connected": True, "front_ultra": 142, "fd_scaled": 245, "lidar": 144,
    "left_ultra": 0xFFFF, "right_ultra": 28, "alpha": 0.95, "sim_speed": 52.0,
    "d_final": 142, "v_rel": -0.15, "a_rel": 0.02, "fcw_state": 0, "bsd_left_state": 0, "bsd_right_state": 0, "bsd_state": 0, "status": 1,
    "frame_count": 0, "last_rx_time": time.time(),
}

d_history = deque(maxlen=MAX_HISTORY)
v_history = deque(maxlen=MAX_HISTORY)

_prev_vrel = -0.15
_prev_time = time.time()
_arel_filtered = 0.02


# =========================================================
# UTILS
# =========================================================
def calc_checksum(data: bytes) -> int:
    cs = 0
    for b in data: cs ^= b
    return cs & 0xFF

def u16_le(frame: bytes, index: int) -> int:
    return frame[index] | (frame[index + 1] << 8)


def s16_le(frame: bytes, index: int) -> int:
    raw = frame[index] | (frame[index + 1] << 8)
    return struct.unpack("<h", struct.pack("<H", raw))[0]

def parse_hmi_frame(frame: bytes):
    if len(frame) != FRAME_SIZE:
        return None

    if frame[0:2] != FRAME_HEADER:
        return None

    if calc_checksum(frame[:-1]) != frame[-1]:
        return None

    front_ultra = u16_le(frame, 2)
    fd_scaled = u16_le(frame, 4)
    lidar = u16_le(frame, 6)
    left_ultra = u16_le(frame, 8)
    right_ultra = u16_le(frame, 10)

    alpha = frame[12] / 100.0
    sim_speed = u16_le(frame, 13) / 100.0

    d_final = u16_le(frame, 15)
    v_rel = s16_le(frame, 17) / 100.0
    a_rel = s16_le(frame, 19) / 100.0

    fcw_state = frame[21]
    bsd_left_state = frame[22]
    bsd_right_state = frame[23]
    status = frame[24]

    # UI cũ dùng bsd_state dạng bitmask
    bsd_state = 0
    if bsd_left_state != 0:
        bsd_state |= 0x01
    if bsd_right_state != 0:
        bsd_state |= 0x02

    return {
        "front_ultra": front_ultra,
        "fd_scaled": fd_scaled,
        "lidar": lidar,
        "left_ultra": left_ultra,
        "right_ultra": right_ultra,
        "alpha": alpha,
        "sim_speed": sim_speed,
        "d_final": d_final,
        "v_rel": v_rel,
        "a_rel": a_rel,
        "fcw_state": fcw_state,
        "bsd_left_state": bsd_left_state,
        "bsd_right_state": bsd_right_state,
        "bsd_state": bsd_state,
        "status": status,
    }

def update_arel(v_rel: float) -> float:
    global _prev_vrel, _prev_time, _arel_filtered
    now = time.time()
    dt = now - _prev_time
    if dt <= 0.001 or dt > 0.5:
        _prev_vrel, _prev_time = v_rel, now
        return _arel_filtered
    raw_a = (v_rel - _prev_vrel) / dt
    _arel_filtered = 0.85 * _arel_filtered + 0.15 * raw_a
    _prev_vrel, _prev_time = v_rel, now
    return _arel_filtered

def dist_text(value: int) -> str:
    return "NO" if value == 0xFFFF else str(value)

def dfinal_color(d: int):
    if d == 0xFFFF: return (155, 155, 160)
    if d <= 35: return (255, 95, 95)
    if d <= 100: return (245, 210, 85)
    return (135, 225, 135)

def vrel_color(v: float):
    if v > 0.05: return (255, 95, 95)
    if v < -0.05: return (135, 225, 135)
    return (228, 228, 232)


# =========================================================
# THREADS
# =========================================================
def serial_worker():
    global running
    if DEMO_MODE:
        demo_worker()
        return

    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.05)
        ser.reset_input_buffer()
        with data_lock:
            latest_data["connected"] = True
            latest_data["last_rx_time"] = time.time()
    except Exception:
        with data_lock: latest_data["connected"] = False
        return

    rx_buffer = bytearray()
    while running:
        try:
            chunk = ser.read(256)
            if chunk: rx_buffer.extend(chunk)
            while len(rx_buffer) >= FRAME_SIZE:
                pos = rx_buffer.find(FRAME_HEADER)
                if pos < 0: rx_buffer.clear(); break
                if pos > 0: del rx_buffer[:pos]
                if len(rx_buffer) < FRAME_SIZE: break

                frame = bytes(rx_buffer[:FRAME_SIZE])
                del rx_buffer[:FRAME_SIZE]
                parsed = parse_hmi_frame(frame)
                if parsed is not None:
                    with data_lock:
                        latest_data.update(parsed)
                        latest_data["connected"] = True
                        latest_data["frame_count"] += 1
                        latest_data["last_rx_time"] = time.time()
        except Exception:
            with data_lock: latest_data["connected"] = False
            time.sleep(0.2)
    try: ser.close()
    except Exception: pass

def demo_worker():
    t = 0.0
    while running:
        t += 0.05
        d = int(142 + 10 * math.sin(t * 0.5))
        v = -0.15 + 0.05 * math.cos(t * 0.8)
        a = 0.02 + 0.01 * math.sin(t * 1.2)
        fd_scaled = int(abs(v) / 0.0062 * 100.0)

        with data_lock:
            latest_data.update({
                "connected": True, "front_ultra": max(4, d), "fd_scaled": fd_scaled,
                "lidar": max(4, d + 2), "left_ultra": 0xFFFF, "right_ultra": 28,
                "alpha": 0.95, "sim_speed": 52.0 + 1.5 * math.sin(t * 0.2), "d_final": max(4, d),
                "v_rel": v, "a_rel": a, "fcw_state": int(t) % 3, "bsd_state": 3, "status": 1,
                "frame_count": latest_data["frame_count"] + 1, "last_rx_time": time.time(),
            })
        time.sleep(0.05)


# =========================================================
# UI DRAWING WIDGETS
# =========================================================
def create_header_ui(tag_bg, tag_text, width, text):
    with dpg.child_window(width=width, height=PANEL_HEADER_H, tag=tag_bg, border=False, no_scrollbar=True):
        dpg.add_spacer(height=8)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=15)
            dpg.add_text(text, tag=tag_text)

def create_trend_header_ui(tag_bg, tag_main, tag_sub, tag_trend, width, main_char, sub_char, title_text):
    with dpg.child_window(width=width, height=PANEL_HEADER_H, tag=tag_bg, border=False, no_scrollbar=True):
        dpg.add_spacer(height=8)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=15)
            dpg.add_text(main_char, tag=tag_main)
            dpg.add_text(sub_char, tag=tag_sub)
            dpg.add_spacer(width=5)
            dpg.add_text(title_text, tag=tag_trend)

def load_front_view_textures():
    try:
        with dpg.texture_registry(show=False):
            if os.path.exists(ROAD_IMAGE_PATH):
                w, h, c, data = dpg.load_image(ROAD_IMAGE_PATH)
                dpg.add_static_texture(w, h, data, tag="tex_front_road")
            else:
                print(f"[HMI] Khong tim thay road.png: {ROAD_IMAGE_PATH}")

            if os.path.exists(BSD_WARNING_IMAGE_PATH):
                w, h, c, data = dpg.load_image(BSD_WARNING_IMAGE_PATH)
                dpg.add_static_texture(w, h, data, tag="tex_bsd_warning")
            else:
                print(f"[HMI] Khong tim thay bsd_warning.png: {BSD_WARNING_IMAGE_PATH}")

            if os.path.exists(CAR_IMAGE_PATH):
                w, h, c, data = dpg.load_image(CAR_IMAGE_PATH)
                dpg.add_static_texture(w, h, data, tag="tex_car_top")
            else:
                print(f"[HMI] Khong tim thay car.png: {CAR_IMAGE_PATH}")

            warning_assets = [
                (GREEN_WARNING_IMAGE_PATH,  "tex_front_warning_green",  "green_warning.png"),
                (YELLOW_WARNING_IMAGE_PATH, "tex_front_warning_yellow", "yellow_warning.png"),
                (RED_WARNING_IMAGE_PATH,    "tex_front_warning_red",    "red_warning.png"),
            ]

            for path, tag, name in warning_assets:
                if os.path.exists(path):
                    w, h, c, data = dpg.load_image(path)
                    dpg.add_static_texture(w, h, data, tag=tag)
                else:
                    print(f"[HMI] Khong tim thay {name}: {path}")

    except Exception as e:
        print(f"[HMI] Loi load anh front view: {e}")

def arc_points(cx, cy, rx, ry, deg0, deg1, segments=32):
    pts = []
    for i in range(segments + 1):
        t = math.radians(deg0 + (deg1 - deg0) * i / segments)
        x = cx + rx * math.cos(t)
        y = cy + ry * math.sin(t)
        pts.append((x, y))
    return pts


def draw_radar_wave(parent, cx, cy, mode, active_level):
    """
    mode:
        "front" = sóng FCW phía trước
        "left"  = sóng BSD trái
        "right" = sóng BSD phải
    """

    if mode == "front":
        if active_level == 2:
            col = (255, 70, 75, 180)
        elif active_level == 1:
            col = (245, 210, 85, 165)
        else:
            col = (100, 230, 135, 120)

        sizes = [(62, 34), (96, 54), (132, 76)]
        for i, (rx, ry) in enumerate(sizes):
            pts = arc_points(cx, cy, rx, ry, 210, 330, 36)
            dpg.draw_polyline(
                pts,
                color=(col[0], col[1], col[2], max(45, col[3] - i * 25)),
                thickness=3,
                parent=parent
            )

    elif mode == "left":
        col = (255, 90, 95, 150) if active_level else (145, 150, 160, 65)

        sizes = [(42, 32), (68, 50), (96, 70)]
        for i, (rx, ry) in enumerate(sizes):
            pts = arc_points(cx, cy, rx, ry, 135, 235, 28)
            dpg.draw_polyline(
                pts,
                color=(col[0], col[1], col[2], max(35, col[3] - i * 18)),
                thickness=3,
                parent=parent
            )

    elif mode == "right":
        col = (255, 90, 95, 150) if active_level else (145, 150, 160, 65)

        sizes = [(42, 32), (68, 50), (96, 70)]
        for i, (rx, ry) in enumerate(sizes):
            pts = arc_points(cx, cy, rx, ry, -55, 45, 28)
            dpg.draw_polyline(
                pts,
                color=(col[0], col[1], col[2], max(35, col[3] - i * 18)),
                thickness=3,
                parent=parent
            )

def draw_front_warning_image(parent, ego_cx, car_y1, fcw_state):
    # FCW/FWC:
    # 0 = normal  -> green
    # 1 = warning -> yellow
    # 2 = critical -> red

    if fcw_state == 2:
        tex_tag = "tex_front_warning_red"
    elif fcw_state == 1:
        tex_tag = "tex_front_warning_yellow"
    else:
        tex_tag = "tex_front_warning_green"

    if not dpg.does_item_exist(tex_tag):
        tex_tag = "tex_front_warning_green"

    if not dpg.does_item_exist(tex_tag):
        return

    warning_w = 210
    warning_h = 360

    x1 = ego_cx - warning_w // 2
    y1 = car_y1 - warning_h + 40
    x2 = ego_cx + warning_w // 2
    y2 = car_y1 + 255

    dpg.draw_image(
        tex_tag,
        (x1, y1),
        (x2, y2),
        parent=parent
    )

def draw_bsd_warning_image(parent, car_x1, car_x2, car_y2, bsd_state):
    if not dpg.does_item_exist("tex_bsd_warning"):
        return

    bsd_left_active = (bsd_state & 0x01) != 0
    bsd_right_active = (bsd_state & 0x02) != 0

    icon_w = 30
    icon_h = 30

    # vị trí cảnh báo ở 2 bên hông sau xe
    y1 = car_y2 - 55
    y2 = y1 + icon_h

    left_x1 = car_x1 - icon_w + 80
    left_x2 = left_x1 + icon_w

    right_x1 = car_x2 - 80
    right_x2 = right_x1 + icon_w

    if bsd_left_active:
        dpg.draw_image(
            "tex_bsd_warning",
            (left_x1, y1),
            (left_x2, y2),
            parent=parent
        )

    if bsd_right_active:
        dpg.draw_image(
            "tex_bsd_warning",
            (right_x1, y1),
            (right_x2, y2),
            parent=parent
        )

def draw_front_view(d_final=0xFFFF, v_rel=0.0, fcw=0, bsd=0):
    if not dpg.does_item_exist("front_draw"):
        return

    dpg.delete_item("front_draw", children_only=True)

    w = TOP_LEFT_W
    h = TOP_H - PANEL_HEADER_H

    # =====================================================
    # CONFIG BỐ CỤC
    # =====================================================
    EGO_CX = 442
    CAR_W  = 250
    CAR_H  = 220
    CAR_Y1 = 220
    CAR_Y2 = CAR_Y1 + CAR_H

    car_x1 = EGO_CX - CAR_W // 2
    car_x2 = EGO_CX + CAR_W // 2

    # =====================================================
    # 1. ROAD BACKGROUND
    # =====================================================
    if dpg.does_item_exist("tex_front_road"):
        dpg.draw_image(
            "tex_front_road",
            (0, 0),
            (w, h),
            parent="front_draw"
        )
    else:
        dpg.draw_rectangle(
            (0, 0), (w, h),
            fill=(7, 10, 18),
            color=(0, 0, 0, 0),
            parent="front_draw"
        )

    # phủ tối nhẹ để nền đỡ gắt
    dpg.draw_rectangle(
        (0, 0), (w, h),
        fill=(0, 0, 0, 18),
        color=(0, 0, 0, 0),
        parent="front_draw"
    )

    # =====================================================
    # 2. CAR IMAGE
    # =====================================================
    draw_front_warning_image("front_draw", EGO_CX, CAR_Y1, fcw)

    if dpg.does_item_exist("tex_car_top"):
        dpg.draw_image(
            "tex_car_top",
            (car_x1, CAR_Y1),
            (car_x2, CAR_Y2),
            parent="front_draw"
        )
    else:
        dpg.draw_rectangle(
            (car_x1 + 24, CAR_Y1 + 20),
            (car_x2 - 24, CAR_Y2 - 20),
            fill=(220, 222, 226),
            color=(245, 245, 245),
            rounding=14,
            parent="front_draw"
        )

    draw_bsd_warning_image(
        "front_draw",
        car_x1,
        car_x2,
        CAR_Y2,
        bsd
    )

    # =====================================================
    # 3. METRICS OVERLAY
    # =====================================================
    metric_x = 835
    metric_y = 72
    line_w = 390

    d_col = dfinal_color(d_final)
    v_col = vrel_color(v_rel)
    a_col = (228, 228, 232)

    with data_lock:
        a_rel_now = latest_data.get("a_rel", 0.0)

    def draw_metric(row_y, main_char, sub_text, value_text, value_color):
        it = dpg.draw_text(
            (metric_x, row_y),
            main_char,
            color=(235, 236, 242),
            size=34,
            parent="front_draw"
        )
        if dpg.does_item_exist("font_metric_main"):
            dpg.bind_item_font(it, "font_metric_main")

        it = dpg.draw_text(
            (metric_x + 30, row_y + 10),
            sub_text,
            color=(235, 236, 242),
            size=20,
            parent="front_draw"
        )
        if dpg.does_item_exist("font_metric_sub"):
            dpg.bind_item_font(it, "font_metric_sub")

        it = dpg.draw_text(
            (metric_x + 145, row_y + 1),
            "=",
            color=(235, 236, 242),
            size=30,
            parent="front_draw"
        )
        if dpg.does_item_exist("font_metric_value"):
            dpg.bind_item_font(it, "font_metric_value")

        it = dpg.draw_text(
            (metric_x + 210, row_y + 1),
            value_text,
            color=value_color,
            size=30,
            parent="front_draw"
        )
        if dpg.does_item_exist("font_metric_value"):
            dpg.bind_item_font(it, "font_metric_value")

    draw_metric(metric_y, "D", "final", f"{dist_text(d_final)} cm", d_col)

    dpg.draw_line(
        (metric_x, metric_y + 46),
        (metric_x + line_w, metric_y + 46),
        color=(100, 104, 116, 120),
        parent="front_draw"
    )

    draw_metric(metric_y + 78, "V", "rel", f"{v_rel:+.2f} m/s", v_col)

    dpg.draw_line(
        (metric_x, metric_y + 124),
        (metric_x + line_w, metric_y + 124),
        color=(100, 104, 116, 120),
        parent="front_draw"
    )

    draw_metric(metric_y + 156, "A", "rel", f"{a_rel_now:+.2f} m/s²", a_col)

# =========================================================
# BUILD UI MAIN
# =========================================================
def build_ui():
    dpg.create_context()
    load_front_view_textures()

    # Load toàn bộ Font Bold
    with dpg.font_registry():
        font_title = dpg.add_font(FONT_BOLD, 36)          
        font_header = dpg.add_font(FONT_BOLD, 26)         
        font_raw_name = dpg.add_font(FONT_BOLD, 26)       
        font_raw_value = dpg.add_font(FONT_BOLD, 28)      
        font_metric_lbl = dpg.add_font(FONT_BOLD, 36)     
        font_metric_val = dpg.add_font(FONT_BOLD, 42)     
        font_plot = dpg.add_font(FONT_UI, 18)
        font_metric_main = dpg.add_font(FONT_METRIC_MAIN, 36, tag="font_metric_main")
        font_metric_sub = dpg.add_font(FONT_METRIC_SUB, 20, tag="font_metric_sub")
        font_metric_value = dpg.add_font(FONT_METRIC_VALUE, 32, tag="font_metric_value")
        font_metric_label = dpg.add_font(FONT_METRIC_VALUE, 15, tag="font_metric_label")

    # Tạo viewport full HD và ép nằm ở màn hình chính
    dpg.create_viewport(
        title="COLLISION WARNING HMI",
        width=WINDOW_W,
        height=WINDOW_H,
        x_pos=0,
        y_pos=0,
        decorated=False,
        resizable=False
    )
    
    with dpg.window(tag="main_window", pos=(0, 0), width=WINDOW_W, height=WINDOW_H, no_title_bar=True, no_move=False): 
        dpg.add_spacer(height=MARGIN_X)

        with dpg.group(horizontal=True):
            dpg.add_spacer(width=MARGIN_X)
            with dpg.group():
                
                # --- TITLE BAR ---
                with dpg.child_window(width=WINDOW_W - 2*MARGIN_X, height=TITLE_BAR_H, tag="main_title_bg", border=False, no_scrollbar=True):
                    dpg.add_spacer(height=10)
                    with dpg.group(horizontal=True):
                        dpg.add_spacer(width=20)
                        dpg.add_text("ADAS COLLISION WARNING HMI", tag="main_title_text")
                
                dpg.add_spacer(height=GAP_Y)

                # ================= TOP ROW =================
                with dpg.group(horizontal=True):
                    
                    # Panel Trái: Cockpit (1280px)
                    with dpg.child_window(width=TOP_LEFT_W, height=TOP_H, border=True, no_scrollbar=True):
                        create_header_ui("front_header_bg", "txt_front_header", TOP_LEFT_W, "FRONT VIEW")
                        dpg.add_drawlist( width=TOP_LEFT_W, height=TOP_H - PANEL_HEADER_H, tag="front_draw")
                            
                            # METRICS BẢNG
                        with dpg.child_window(width=400, height=360, border=False, no_scrollbar=True):
                                dpg.add_spacer(height=35)
                                with dpg.table(header_row=False, borders_innerH=True, policy=dpg.mvTable_SizingFixedFit):
                                    dpg.add_table_column(init_width_or_weight=110)
                                    dpg.add_table_column(init_width_or_weight=50)
                                    dpg.add_table_column(init_width_or_weight=200)
                                    
                                    with dpg.table_row():
                                        dpg.add_text("D_final", tag="lbl_d_main")
                                        dpg.add_text("=", tag="lbl_d_eq")
                                        dpg.add_text("---", tag="val_dfinal")
                                    with dpg.table_row():
                                        dpg.add_text("V_rel", tag="lbl_v_main")
                                        dpg.add_text("=", tag="lbl_v_eq")
                                        dpg.add_text("---", tag="val_vrel")
                                    with dpg.table_row():
                                        dpg.add_text("A_rel", tag="lbl_a_main")
                                        dpg.add_text("=", tag="lbl_a_eq")
                                        dpg.add_text("---", tag="val_arel")

                    dpg.add_spacer(width=GAP_X)

                    # Panel Phải: Raw Data 
                    with dpg.child_window(width=TOP_RIGHT_W, height=TOP_H, border=True, no_scrollbar=True):
                        create_header_ui("raw_header_bg", "txt_raw_header", TOP_RIGHT_W, "RAW SENSOR DATA")
                        dpg.add_spacer(height=15) 
                        with dpg.group(horizontal=True):
                            dpg.add_spacer(width=20) 
                            with dpg.table(tag="raw_table", width=TOP_RIGHT_W-40, header_row=False, policy=dpg.mvTable_SizingFixedFit):
                                dpg.add_table_column(init_width_or_weight=290)
                                dpg.add_table_column(init_width_or_weight=200)
                                raw_rows = [
                                    ("Alpha (α)", "lbl_alpha"), ("LiDAR", "lbl_lidar"),
                                    ("Front Ultra", "lbl_front_ultra"), ("Left Ultra", "lbl_left_ultra"),
                                    ("Right Ultra", "lbl_right_ultra"), ("Doppler Radar", "lbl_fd"), 
                                    ("Sim Speed", "lbl_sim_speed"), ("Node Status", "lbl_node_status"),
                                ]
                                for name, tag in raw_rows:
                                    with dpg.table_row():
                                        dpg.add_text(name, tag=f"{tag}_name")
                                        dpg.add_text("---", tag=tag)

                dpg.add_spacer(height=GAP_Y)

                # ================= BOTTOM ROW =================
                with dpg.group(horizontal=True):
                    
                    # Plot 1
                    with dpg.child_window(width=BOT_W, height=BOT_H, border=True, no_scrollbar=True):
                        create_trend_header_ui("dplot_bg", "txt_dplot_m", "txt_dplot_s", "txt_dplot_t", BOT_W, "DISTANCE", "", "TREND")
                        dpg.add_spacer(height=15)
                        with dpg.group(horizontal=True):
                            dpg.add_spacer(width=16)
                            with dpg.plot(width=BOT_W - 32, height=BOT_H - PANEL_HEADER_H - 30, tag="plot_dfinal"):
                                dpg.add_plot_axis(dpg.mvXAxis, label="history index", tag="x_axis_d")
                                dpg.add_plot_axis(dpg.mvYAxis, label="D_final (cm)", tag="y_axis_d")
                                dpg.add_inf_line_series([1.0], parent="x_axis_d", tag="axisline_x_d")
                                dpg.add_inf_line_series([0.0], parent="y_axis_d", tag="axisline_y_d")
                                dpg.add_line_series([], [], parent="y_axis_d", tag="series_dfinal")
                                dpg.add_scatter_series([], [], parent="y_axis_d", tag="series_dfinal_start")

                    dpg.add_spacer(width=GAP_X)

                    # Plot 2
                    with dpg.child_window(width=BOT_W, height=BOT_H, border=True, no_scrollbar=True):
                        create_trend_header_ui("vplot_bg", "txt_vplot_m", "txt_vplot_s", "txt_vplot_t", BOT_W, "RELATIVE VELOCITY", "", "TREND")
                        dpg.add_spacer(height=15)
                        with dpg.group(horizontal=True):
                            dpg.add_spacer(width=16)
                            with dpg.plot(width=BOT_W - 32, height=BOT_H - PANEL_HEADER_H - 30, tag="plot_vrel"):
                                dpg.add_plot_axis(dpg.mvXAxis, label="history index", tag="x_axis_v")
                                dpg.add_plot_axis(dpg.mvYAxis, label="V_rel (m/s)", tag="y_axis_v")
                                dpg.add_inf_line_series([1.0], parent="x_axis_v", tag="axisline_x_v")
                                dpg.add_inf_line_series([0.0], parent="y_axis_v", tag="axisline_y_v")
                                dpg.add_line_series([], [], parent="y_axis_v", tag="series_vrel")
                                dpg.add_scatter_series([], [], parent="y_axis_v", tag="series_vrel_start")


    # =====================================================
    # THEME ENGINE 
    # =====================================================
    with dpg.theme() as global_theme:
        with dpg.theme_component(dpg.mvAll):
            dpg.add_theme_color(dpg.mvThemeCol_WindowBg, (8, 10, 16))
            dpg.add_theme_color(dpg.mvThemeCol_ChildBg, (14, 16, 24))
            dpg.add_theme_color(dpg.mvThemeCol_Border, (56, 60, 72))
            dpg.add_theme_color(dpg.mvThemeCol_Text, (225, 226, 232))
            
            dpg.add_theme_style(dpg.mvStyleVar_WindowPadding, 0, 0)
            dpg.add_theme_style(dpg.mvStyleVar_ItemSpacing, 0, 0) 
            dpg.add_theme_style(dpg.mvStyleVar_ChildRounding, 6)
            dpg.add_theme_style(dpg.mvStyleVar_CellPadding, 8, 11) 

    with dpg.theme() as header_theme:
        with dpg.theme_component(dpg.mvChildWindow):
            dpg.add_theme_color(dpg.mvThemeCol_ChildBg, (110, 115, 126, 50))
            dpg.add_theme_style(dpg.mvStyleVar_ChildRounding, 4)

    dpg.bind_theme(global_theme)
    for bg in ["main_title_bg", "front_header_bg", "raw_header_bg", "dplot_bg", "vplot_bg"]:
        dpg.bind_item_theme(bg, header_theme)

    try:
        with dpg.theme() as line_d_theme:
            with dpg.theme_component(dpg.mvLineSeries):
                dpg.add_theme_color(dpg.mvPlotCol_Line, (135, 215, 90), category=dpg.mvThemeCat_Plots)
        with dpg.theme() as line_v_theme:
            with dpg.theme_component(dpg.mvLineSeries):
                dpg.add_theme_color(dpg.mvPlotCol_Line, (235, 205, 80), category=dpg.mvThemeCat_Plots)
        with dpg.theme() as dot_d_theme:
            with dpg.theme_component(dpg.mvScatterSeries):
                dpg.add_theme_color(dpg.mvPlotCol_MarkerFill, (135, 215, 90), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_MarkerOutline, (135, 215, 90), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_Marker, dpg.mvPlotMarker_Circle, category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerSize, 3, category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerWeight, 1, category=dpg.mvThemeCat_Plots)
        with dpg.theme() as dot_v_theme:
            with dpg.theme_component(dpg.mvScatterSeries):
                dpg.add_theme_color(dpg.mvPlotCol_MarkerFill, (235, 205, 80), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_MarkerOutline, (235, 205, 80), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_Marker, dpg.mvPlotMarker_Circle, category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerSize, 3, category=dpg.mvThemeCat_Plots)
                dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerWeight, 1, category=dpg.mvThemeCat_Plots)
        with dpg.theme() as axis_line_theme:
            with dpg.theme_component(dpg.mvInfLineSeries):
                dpg.add_theme_color(dpg.mvPlotCol_Line, (120, 124, 136), category=dpg.mvThemeCat_Plots)
        with dpg.theme() as plot_area_theme:
            with dpg.theme_component(dpg.mvPlot):
                dpg.add_theme_color(dpg.mvPlotCol_FrameBg, (14, 16, 24), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_PlotBg, (6, 8, 14), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_PlotBorder, (90, 94, 106), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_AxisText, (210, 212, 220), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_AxisGrid, (70, 74, 86, 70), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_AxisTick, (118, 122, 134), category=dpg.mvThemeCat_Plots)
                dpg.add_theme_color(dpg.mvPlotCol_TitleText, (225, 226, 232), category=dpg.mvThemeCat_Plots)
        dpg.bind_item_theme("series_dfinal", line_d_theme)
        dpg.bind_item_theme("series_vrel", line_v_theme)
        dpg.bind_item_theme("series_dfinal_start", dot_d_theme)
        dpg.bind_item_theme("series_vrel_start", dot_v_theme)
        for tag in ["axisline_x_d", "axisline_y_d", "axisline_x_v", "axisline_y_v"]:
            dpg.bind_item_theme(tag, axis_line_theme)
        dpg.bind_item_theme("plot_dfinal", plot_area_theme)
        dpg.bind_item_theme("plot_vrel", plot_area_theme)
    except Exception: pass

    # =====================================================
    # BIND FONT
    # =====================================================
    dpg.bind_item_font("main_title_text", font_title)
    dpg.bind_item_font("txt_front_header", font_header)
    dpg.bind_item_font("txt_raw_header", font_header)
    
    for t in ["txt_dplot_m", "txt_dplot_t", "txt_vplot_m", "txt_vplot_t"]:
        dpg.bind_item_font(t, font_header)

    for tag in ["lbl_front_ultra", "lbl_left_ultra", "lbl_right_ultra", "lbl_lidar", "lbl_fd", "lbl_alpha", "lbl_sim_speed", "lbl_node_status"]:
        dpg.bind_item_font(f"{tag}_name", font_raw_name)
        dpg.bind_item_font(tag, font_raw_value)
        dpg.configure_item(tag, color=(145, 225, 145))
        dpg.configure_item(f"{tag}_name", color=(240, 240, 245))

    for t in ["lbl_d_main", "lbl_d_eq", "lbl_v_main", "lbl_v_eq", "lbl_a_main", "lbl_a_eq"]:
        dpg.bind_item_font(t, font_metric_lbl)
        dpg.configure_item(t, color=(240, 242, 250))
    for t in ["val_dfinal", "val_vrel", "val_arel"]:
        dpg.bind_item_font(t, font_metric_val)
        
    dpg.bind_item_font("plot_dfinal", font_plot)
    dpg.bind_item_font("plot_vrel", font_plot)

    try:
        dpg.set_axis_limits("x_axis_d", 1, MAX_HISTORY)
        dpg.set_axis_limits("y_axis_d", 0, 500)
        dpg.set_axis_limits("x_axis_v", 1, MAX_HISTORY)
        dpg.set_axis_limits("y_axis_v", -3.0, 3.0)
    except Exception:
        pass

    draw_front_view()

    with dpg.handler_registry():
        dpg.add_key_press_handler(key=dpg.mvKey_Escape, callback=lambda: dpg.stop_dearpygui())

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.set_viewport_pos([0, 0])
    dpg.set_primary_window("main_window", True)


# =========================================================
# UPDATE UI
# =========================================================
def update_ui():
    with data_lock:
        data = latest_data.copy()

    d = data["d_final"]
    v = data["v_rel"]
    a = data["a_rel"]
    fd_hz = data["fd_scaled"] / 100.0

    dpg.set_value("lbl_front_ultra", f"{dist_text(data['front_ultra'])} cm")
    dpg.set_value("lbl_left_ultra",  f"{dist_text(data['left_ultra'])} cm")
    dpg.set_value("lbl_right_ultra", f"{dist_text(data['right_ultra'])} cm")
    dpg.set_value("lbl_lidar",       f"{dist_text(data['lidar'])} cm")
    dpg.set_value("lbl_fd",          f"{fd_hz:.2f} Hz")
    dpg.set_value("lbl_alpha",       f"{data['alpha']:.2f}")
    dpg.set_value("lbl_sim_speed",   f"{data['sim_speed']:.2f} m/s")
    dpg.set_value("lbl_node_status", "OK" if data["status"] == 1 else "ERR")

    dpg.set_value("val_dfinal", f"{dist_text(d)} cm")
    dpg.configure_item("val_dfinal", color=dfinal_color(d))

    dpg.set_value("val_vrel", f"{v:+.2f} m/s")
    dpg.configure_item("val_vrel", color=vrel_color(v))

    dpg.set_value("val_arel", f"{a:+.2f} m/s²")
    dpg.configure_item("val_arel", color=(228, 228, 232))

    d_value = 0 if d == 0xFFFF else d
    d_history.append(d_value)
    v_history.append(v)

    xs_d = list(range(1, len(d_history) + 1))
    xs_v = list(range(1, len(v_history) + 1))

    dpg.set_value("series_dfinal", [xs_d, list(d_history)])
    dpg.set_value("series_vrel", [xs_v, list(v_history)])

    dpg.set_axis_limits("x_axis_d", 1, MAX_HISTORY)
    dpg.set_axis_limits("y_axis_d", 0, 500)
    dpg.set_axis_limits("x_axis_v", 1, MAX_HISTORY)
    dpg.set_axis_limits("y_axis_v", -3.0, 3.0)

    draw_front_view(d, v, data["fcw_state"], data["bsd_state"])


# =========================================================
# MAIN
# =========================================================
def main():
    global running
    build_ui()
    worker = threading.Thread(target=serial_worker, daemon=True)
    worker.start()
    while dpg.is_dearpygui_running():
        update_ui()
        dpg.render_dearpygui_frame()
        time.sleep(0.02)
    running = False
    dpg.destroy_context()

if __name__ == "__main__":
    main()