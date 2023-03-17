#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "ui_core.h"
#include "os_linux.h"

#define HASH_H_IMPLEMENTATION
#include "utils/hash.h"


UI_Widget* widget_root;
UI_Widget* widget_active_parent;

UI_Comm UI_Button(char* string) {
    UI_Widget* widget = UI_WidgetMake(
        UI_WidgetFlag_Clickable|
        UI_WidgetFlag_DrawBorder|
        UI_WidgetFlag_DrawText|
        UI_WidgetFlag_DrawBackground|
        UI_WidgetFlag_HotAnimation|
        UI_WidgetFlag_ActiveAnimation,
        string
    );
    return UI_CommFromWidget(widget);
}

UI_Widget* UI_WidgetMake(UI_WidgetFlags flags, char* string) {
    UI_Widget* widget = mem_push(UI_Widget);

    widget->flags = flags;
    widget->string = mem_cpy_str(string);

    widget->parent = widget_active_parent;
    UI_Widget* last_node = widget_active_parent->last;
    if (last_node == 0) {
        widget_active_parent->first = widget;
        widget_active_parent->last = widget;
    } else {
        widget->prev = last_node;
        last_node->next = widget;
    }
    widget_active_parent->last = widget;

    // TODO: add widget to the hashtable


    // TODO: add sizes to widget
    UI_Size size_x = widget->semantic_size[0];
    UI_Size size_y = widget->semantic_size[1];

    size_x.kind = UI_SizeKind_PercentOfParent;
    size_x.value = 1.0;

    size_y.kind = UI_SizeKind_PercentOfParent;
    size_y.value = 1.0;

    return widget;
}

UI_Comm UI_CommFromWidget(UI_Widget *widget) {
    return *mem_push(UI_Comm);
}

UI_Widget* UI_PushParent(UI_Widget *widget) {
    widget_active_parent = widget;
    return widget;
}
UI_Widget* UI_PopParent(void) {
    UI_Widget* new_parent = widget_active_parent->parent;
    if (new_parent == 0) {
        return widget_active_parent;
    }
    widget_active_parent = new_parent;
    return new_parent;
}


int main() {
    widget_root = mem_push(UI_Widget);
    widget_active_parent = widget_root;

    UI_Widget* first = UI_WidgetMake(0, "New item1");

    UI_Widget* second = UI_WidgetMake(0, "New item2");
    UI_WidgetMake(0, "New item3");

    UI_PushParent(UI_WidgetMake(0, "New parent"));
    UI_WidgetMake(0, "New child1");
    UI_WidgetMake(0, "New child2");

    UI_Widget* head = widget_root;
    while (head->first != 0) {
        head = head->first;
        while(head != 0) {
            printf("%s <=> ", head->string);
            if (head->next == 0) {
                printf("\n");
                printf("End\n");
                break;
            }
            head = head->next;
        }
    }


    return 0;
}
