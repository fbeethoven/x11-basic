#ifndef UI_CORE_H
#define UI_CORE_H

#include "common.h"


typedef enum UI_SizeKind {
  UI_SizeKind_Null,
  UI_SizeKind_Pixels,
  UI_SizeKind_TextContent,
  UI_SizeKind_PercentOfParent,
  UI_SizeKind_ChildrenSum,
} UI_SizeKind;

typedef struct UI_Size {
    UI_SizeKind kind;
    float value;
    float strictness;
} UI_Size;

typedef u32 UI_WidgetFlags;
enum {
  UI_WidgetFlag_Clickable       = (1<<0),
  UI_WidgetFlag_ViewScroll      = (1<<1),
  UI_WidgetFlag_DrawText        = (1<<2),
  UI_WidgetFlag_DrawBorder      = (1<<3),
  UI_WidgetFlag_DrawBackground  = (1<<4),
  UI_WidgetFlag_DrawDropShadow  = (1<<5),
  UI_WidgetFlag_Clip            = (1<<6),
  UI_WidgetFlag_HotAnimation    = (1<<7),
  UI_WidgetFlag_ActiveAnimation = (1<<8),
};

typedef u64 UI_Key;

typedef struct UI_Widget UI_Widget;
struct UI_Widget {
    // tree links
    UI_Widget *first;
    UI_Widget *last;
    UI_Widget *next;
    UI_Widget *prev;
    UI_Widget *parent;

    // hash links
    UI_Widget *hash_next;
    UI_Widget *hash_prev;

    // key+generation info
    UI_Key key;
    u64 last_frame_touched_index;

    // per-frame info provided by builders
    UI_WidgetFlags flags;
    char* string;
    UI_Size semantic_size[2];

    // computed every frame
    float computed_rel_position[2];
    float computed_size[2];
    float rect[2];

    // persistent data
    float hot_t;
    float active_t;
};

// basic key type helpers
UI_Key UI_KeyNull(void);
UI_Key UI_KeyFromString(char* string);
u32 UI_KeyMatch(UI_Key a, UI_Key b);

UI_Widget *UI_WidgetMake(UI_WidgetFlags flags, char* string);

void UI_WidgetEquipDisplayString(UI_Widget* widget, char* string);
void UI_WidgetEquipChildLayoutAxis(UI_Widget* widget, u8 axis);

UI_Widget *UI_PushParent(UI_Widget *widget);
UI_Widget *UI_PopParent(void);

// shor for UI_Communication
typedef struct UI_Comm
{
  UI_Widget *widget;
  float mouse[2];
  float drag_delta[2];
  u8 clicked;
  u8 double_clicked;
  u8 right_clicked;
  u8 pressed;
  u8 released;
  u8 dragging;
  u8 hovering;
} UI_Comm;

// and finally, the call that implements all interaction
// for all widgets - "get the user communication from
// this widget":
UI_Comm UI_CommFromWidget(UI_Widget *widget);

UI_Comm UI_Button(char* string);

#endif  // UI_CORE_H
