// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "filesystem_generator_window.h"
#include "cb_filesystem_generator_window.h"
#include "gui_strings.h"

#include "fl_dnd_box.h"

filesystem_generator_window::filesystem_generator_window() {
  { window = new Fl_Double_Window(705, 415, getString(STR_FSGENERATORWINDOW_0001));
    window->user_data((void*)(this));
    { bt_injectdir = new Fl_Button(610, 20, 90, 25, getString(STR_FSGENERATORWINDOW_0002));
      bt_injectdir->labelsize(12);
      bt_injectdir->callback((Fl_Callback*)filesystem_generator_window_bt_injectdir);
    } // Fl_Button* bt_injectdir
    { bt_cancel = new Fl_Button(610, 379, 90, 25, getString(STR_FSGENERATORWINDOW_0003));
      bt_cancel->labelsize(12);
      bt_cancel->callback((Fl_Callback*)filesystem_generator_window_bt_close);
    } // Fl_Button* bt_cancel
    { choice_filesystype = new Fl_Choice(5, 20, 600, 25, getString(STR_FSGENERATORWINDOW_0004));
      choice_filesystype->down_box(FL_BORDER_BOX);
      choice_filesystype->labelsize(12);
      choice_filesystype->textsize(12);
      choice_filesystype->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Choice* choice_filesystype
    { fs_browser = new Fl_Tree(5, 60, 600, 310, getString(STR_FSGENERATORWINDOW_0005));
      fs_browser->labeltype(FL_NO_LABEL);
      fs_browser->callback((Fl_Callback*)filesystem_generator_window_browser_fs);
    } // Fl_Tree* fs_browser
    { bt_delete = new Fl_Button(610, 90, 90, 25, getString(STR_FSGENERATORWINDOW_0006));
      bt_delete->callback((Fl_Callback*)filesystem_generator_window_bt_delete);
    } // Fl_Button* bt_delete
    { bt_get = new Fl_Button(610, 60, 90, 25, getString(STR_FSGENERATORWINDOW_0007));
      bt_get->callback((Fl_Callback*)filesystem_generator_window_bt_getfiles);
    } // Fl_Button* bt_get
    { txtout_freesize = new Fl_Output(5, 377, 600, 32);
      txtout_freesize->labeltype(FL_NO_LABEL);
    } // Fl_Output* txtout_freesize
    { hlptxt = new Fl_Output(610, 118, 90, 155);
      hlptxt->box(FL_NO_BOX);
      hlptxt->labeltype(FL_NO_LABEL);
      hlptxt->labelsize(12);
      hlptxt->textsize(12);
      hlptxt->align(Fl_Align(FL_ALIGN_CENTER));
    } // Fl_Output* hlptxt
    { bt_saveexport = new Fl_Button(610, 342, 90, 25, getString(STR_FSGENERATORWINDOW_0008));
      bt_saveexport->callback((Fl_Callback*)filesystem_generator_window_bt_saveexport);
    } // Fl_Button* bt_saveexport
    { bt_loadimage = new Fl_Button(610, 314, 90, 25, getString(STR_FSGENERATORWINDOW_0009));
      bt_loadimage->callback((Fl_Callback*)filesystem_generator_window_bt_loadimage);
    } // Fl_Button* bt_loadimage
    { disk_selector = new Fl_Counter(610, 290, 90, 20, getString(STR_FSGENERATORWINDOW_0010));
      disk_selector->minimum(0);
      disk_selector->maximum(1000);
      disk_selector->step(1);
      disk_selector->callback((Fl_Callback*)filesystem_generator_window_sel_disk);
      disk_selector->align(Fl_Align(FL_ALIGN_TOP));
    } // Fl_Counter* disk_selector
    Fl_DND_Box *o = new Fl_DND_Box(5, 60, 600, 310, 0);
    o->callback(dnd_fs_cb);
	FATAccessInProgress = 0;
    window->end();
  } // Fl_Double_Window* window
}
