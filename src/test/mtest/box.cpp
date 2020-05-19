/*
 * box.cpp
 *
 *  Created on: 18 мая 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/test-fw/mtest.h>
#include <lsp-plug.in/tk/tk.h>

MTEST_BEGIN("tk", box)

    static status_t slot_close(tk::Widget *sender, void *ptr, void *data)
    {
        sender->display()->quit_main();
        return STATUS_OK;
    }

    static status_t slot_key_up(tk::Widget *sender, void *ptr, void *data)
    {
        tk::Window *wnd = tk::widget_cast<tk::Window>(sender);
        ws::event_t *ev = static_cast<ws::event_t *>(data);
        test_type_t *_this = static_cast<test_type_t *>(ptr);

        if ((wnd != NULL) && (ev != NULL) && (ev->nType == ws::UIE_KEY_UP))
        {
            _this->printf("Key up: %c (0x%x)\n", (char)ev->nCode, int(ev->nCode));

            if ((ev->nCode == '+') || (ev->nCode == ws::WSK_KEYPAD_ADD))
                wnd->scaling()->set(wnd->scaling()->get() + 0.25f);
            else if ((ev->nCode == '-') || (ev->nCode == ws::WSK_KEYPAD_SUBTRACT))
                wnd->scaling()->set(wnd->scaling()->get() - 0.25f);
        }
        return STATUS_OK;
    }

    MTEST_MAIN
    {
        tk::Display *dpy = new tk::Display();
        MTEST_ASSERT(dpy != NULL);

        MTEST_ASSERT(dpy->init(0, NULL) == STATUS_OK);

        lltl::parray<tk::Widget> widgets;
        tk::Widget *w = NULL;
        tk::Window *wnd = new tk::Window(dpy);
        tk::Void *wv = NULL;
        tk::Box *box = NULL;
        tk::WidgetContainer *parent = NULL;

        // Initialize window
        MTEST_ASSERT(wnd->init() == STATUS_OK);
        MTEST_ASSERT(wnd->title()->set_raw("Test box") == STATUS_OK);
        MTEST_ASSERT(wnd->role()->set_raw("box_test") == STATUS_OK);
        wnd->bg_color()->set_rgb(0, 0.75, 1.0);
        wnd->actions()->set_actions(ws::WA_MOVE | ws::WA_RESIZE | ws::WA_CLOSE);
        wnd->border_style()->set(ws::BS_DIALOG);
        wnd->size_constraints()->set(160, 100, 640, 400);
        wnd->size()->set(320, 200);
        wnd->slot(tk::SLOT_CLOSE)->bind(slot_close, this);
        wnd->slot(tk::SLOT_KEY_UP)->bind(slot_key_up, this);
        wnd->pointer()->set(ws::MP_TABLE_CELL);
        wnd->layout()->set(-0.5, 0.5, 0.5, 0.5);
        MTEST_ASSERT(widgets.push(wnd));

        // Create vertical box
        MTEST_ASSERT(box = new tk::Box(dpy));
        MTEST_ASSERT(box->init() == STATUS_OK);
        MTEST_ASSERT(widgets.push(box));
        MTEST_ASSERT(wnd->add(box) == STATUS_OK);
        box->bg_color()->set_rgb(1.0f, 1.0f, 1.0f);
        box->orientation()->set_vertical();
        box->spacing()->set(16);
        box->padding()->set_all(8);
        box->pointer()->set(ws::MP_IBEAM);
        parent = box;

        // Create horizontal box (1)
        MTEST_ASSERT(box = new tk::Box(dpy));
        MTEST_ASSERT(box->init() == STATUS_OK);
        MTEST_ASSERT(widgets.push(box));
        MTEST_ASSERT(parent->add(box) == STATUS_OK);
        box->padding()->set_all(1);
        box->bg_color()->set_rgb(1.0, 0.75, 1.0);
        box->pointer()->set(ws::MP_DEFAULT);
//        box->bg_color()->set_rgb(1.0, 1.0, 1.0);
        box->spacing()->set(8);
        box->orientation()->set_horizontal();
        {
            // Create widget (1)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, -1, -1);
            wv->bg_color()->set_rgb(1.0f, 0.0f, 0.0f);
            wv->allocation()->set_expand();
            wv->pointer()->set(ws::MP_HAND);

//            // Create widget (2)
//            MTEST_ASSERT(wv = new tk::Void(dpy));
//            MTEST_ASSERT(wv->init() == STATUS_OK);
//            MTEST_ASSERT(widgets.push(wv));
//            MTEST_ASSERT(box->add(wv) == STATUS_OK);
//            wv->constraints()->set(32, 32, 32, 32);
//            wv->bg_color()->set_rgb(1.0f, 1.0f, 0.0f);
//            wv->allocation()->set_fill();

//            // Create widget (3)
//            MTEST_ASSERT(wv = new tk::Void(dpy));
//            MTEST_ASSERT(wv->init() == STATUS_OK);
//            MTEST_ASSERT(widgets.push(wv));
//            MTEST_ASSERT(box->add(wv) == STATUS_OK);
//            wv->constraints()->set(32, 32, 64, 64);
//            wv->bg_color()->set_rgb(0.0f, 1.0f, 0.0f);
//            wv->allocation()->set_expand();
        }
/*
        // Create horizontal box (2)
        MTEST_ASSERT(box = new tk::Box(dpy));
        MTEST_ASSERT(box->init() == STATUS_OK);
        MTEST_ASSERT(widgets.push(box));
        MTEST_ASSERT(parent->add(box) == STATUS_OK);
        box->bg_color()->set_rgb(0, 0.75, 1.0);
        box->orientation()->set_horizontal();
        box->homogeneous()->set(true);
        box->spacing()->set(8);
        {
            // Create widget (1)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, -1, -1);
            wv->bg_color()->set_rgb(1.0f, 1.0f, 0.0f);
            wv->allocation()->set_fill();
            wv->padding()->set_all(8);

            // Create widget (2)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, 32, 32);
            wv->bg_color()->set_rgb(0.0f, 1.0f, 1.0f);
            wv->allocation()->set_fill(false);

            // Create widget (3)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, 64, 64);
            wv->bg_color()->set_rgb(0.0f, 0.0f, 1.0f);
            wv->allocation()->set_fill();
            wv->padding()->set_all(8);
        }

        // Create horizontal box (3)
        MTEST_ASSERT(box = new tk::Box(dpy));
        MTEST_ASSERT(box->init() == STATUS_OK);
        MTEST_ASSERT(widgets.push(box));
        MTEST_ASSERT(parent->add(box) == STATUS_OK);
        box->bg_color()->set_rgb(0, 0.75, 1.0);
        box->orientation()->set_horizontal();
        {
            // Create widget (1)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, -1, -1);
            wv->bg_color()->set_rgb(0.0f, 1.0f, 0.0f);
            wv->allocation()->set_fill(false);

            // Create widget (2)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, 32, 32);
            wv->bg_color()->set_rgb(0.0f, 1.0f, 0.0f);
            wv->allocation()->set_fill(false);

            // Create widget (3)
            MTEST_ASSERT(wv = new tk::Void(dpy));
            MTEST_ASSERT(wv->init() == STATUS_OK);
            MTEST_ASSERT(widgets.push(wv));
            MTEST_ASSERT(box->add(wv) == STATUS_OK);
            wv->constraints()->set(32, 32, 64, 64);
            wv->bg_color()->set_rgb(0.0f, 1.0f, 1.0f);
            wv->allocation()->set_fill(false);
        }
*/
        // Show window
        wnd->visibility()->set(true);

        MTEST_ASSERT(dpy->main() == STATUS_OK);

        while ((w = widgets.pop()) != NULL)
        {
            w->destroy();
            delete w;
        }

        dpy->destroy();
        delete dpy;
    }

MTEST_END



