// generated by Fast Light User Interface Designer (fluid) version 1.0110

#ifndef batch_converter_window_h
#define batch_converter_window_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>

extern void batch_converter_window_bt_select_src(Fl_Button*, void*);
extern void batch_converter_window_bt_select_dst(Fl_Button*, void*);
extern void batch_converter_window_bt_convert(Fl_Button*, void*);
extern void batch_converter_window_bt_cancel(Fl_Button*, void*);

class batch_converter_window {
public:
  batch_converter_window();
  Fl_Double_Window *window;
  Fl_Choice *choice_file_format;
  Fl_Input *strin_src_dir;
  Fl_Input *strin_dst_dir;
  Fl_Output *strout_convert_status;
  Fl_Button *bt_select_src;
  Fl_Button *bt_select_dst;
  Fl_Button *bt_convert;
  Fl_Button *bt_cancel;
};
#endif