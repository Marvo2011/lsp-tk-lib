/*
 * PopupWindow.h
 *
 *  Created on: 16 июн. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TK_WIDGETS_CONTAINERS_POPUPWINDOW_H_
#define LSP_PLUG_IN_TK_WIDGETS_CONTAINERS_POPUPWINDOW_H_

namespace lsp
{
    namespace tk
    {
        /**
         * Popup window used for different pop-up elements like combo boxes, menus, etc
         */
        class PopupWindow: public Window
        {
            private:
                Window & operator = (const Window &);

            public:
                static const w_class_t          metadata;

            protected:
                prop::Rectangle                 sTrgArea;           // The restricted area where window should never appear
                prop::WidgetPtr<Widget>         sTrgWidget;         // The widget triggered the show
                prop::Integer                   sTrgScreen;         // The target screen to appear

            protected:
                virtual void                    hide_widget();
                virtual void                    show_widget();
                virtual status_t                post_init();
                bool                            init_window(Window *actor);

            public:
                explicit PopupWindow(Display *dpy);
                virtual ~PopupWindow();

                virtual status_t                init();

            public:
                Rectangle                      *trigger_area()              { return &sTrgArea;     }
                const Rectangle                *trigger_area() const        { return &sTrgArea;     }

                Integer                        *trigger_screen()            { return &sTrgScreen;   }
                const Integer                  *trigger_screen() const      { return &sTrgScreen;   }

                WidgetPtr<Widget>              *trigger_widget()            { return &sTrgWidget;   }
                const WidgetPtr<Widget>        *trigger_widget() const      { return &sTrgWidget;   }
        };
    }
}



#endif /* LSP_PLUG_IN_TK_WIDGETS_CONTAINERS_POPUPWINDOW_H_ */
