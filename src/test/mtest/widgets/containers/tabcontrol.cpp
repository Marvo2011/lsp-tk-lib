/*
 * Copyright (C) 2022 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2022 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 13 нояб. 2022 г.
 *
 * lsp-tk-lib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-tk-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-tk-lib. If not, see <https://www.gnu.org/licenses/>.
 */

#include <lsp-plug.in/test-fw/mtest.h>
#include <lsp-plug.in/tk/tk.h>
#include <private/mtest/tk/common.h>

MTEST_BEGIN("tk.widgets.containers", tabcontrol)
    typedef struct handler_t
    {
        test_type_t    *test;
        char           *label;
    } handler_t;

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
                wnd->style()->schema()->scaling()->add(0.25f);
            else if ((ev->nCode == '-') || (ev->nCode == ws::WSK_KEYPAD_SUBTRACT))
                wnd->style()->schema()->scaling()->sub(0.25f);
        }
        return STATUS_OK;
    }

    static status_t slot_mouse_in(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_IN: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_out(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_OUT: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_move(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_MOVE: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_down(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_DOWN: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_up(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_UP: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_click(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_CLICK: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_dbl_click(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_DBL_CLICK: %s\n", h->label);

        return STATUS_OK;
    }

    static status_t slot_mouse_tri_click(tk::Widget *sender, void *ptr, void *data)
    {
        handler_t *h = static_cast<handler_t *>(ptr);
        h->test->printf("MOUSE_TRI_CLICK: %s\n", h->label);

        return STATUS_OK;
    }

    static void destroy_handlers(lltl::parray<handler_t> &vh)
    {
        for (size_t i=0, n=vh.size(); i<n; ++i)
        {
            handler_t *h = vh.uget(i);
            ::free(h->label);
            delete h;
        }
    }

    status_t init_widget(tk::Widget *w, lltl::parray<handler_t> &vh, const char *label)
    {
        status_t res = w->init();
        if (res != STATUS_OK)
            return res;

        handler_t *h = new handler_t;
        if ((h == NULL) || (!vh.add(h)))
            return STATUS_NO_MEM;
        h->test     = this;
        h->label    = ::strdup(label);

        tk::handler_id_t hid;
        hid = w->slots()->bind(tk::SLOT_MOUSE_IN, slot_mouse_in, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_DOWN, slot_mouse_down, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_MOVE, slot_mouse_move, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_UP, slot_mouse_up, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_CLICK, slot_mouse_click, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_DBL_CLICK, slot_mouse_dbl_click, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_TRI_CLICK, slot_mouse_tri_click, h);
        if (hid >= 0) hid = w->slots()->bind(tk::SLOT_MOUSE_OUT, slot_mouse_out, h);

        if (hid < 0)
            res = -hid;

        return res;
    }

    status_t fill_tab_control(tk::TabControl *tc,
        lltl::parray<tk::Widget> & widgets, lltl::parray<handler_t> & vh,
        const LSPString *id)
    {
        LSPString tab_id, full_id;
        tk::Grid *grid;
        tk::Tab *tab;
        tk::Label *label;

        for (size_t i=0; i<3; ++i)
        {
            // Create Tab
            MTEST_ASSERT(tab_id.fmt_ascii("Tab %d", int(i)));
            MTEST_ASSERT(full_id.fmt_ascii("%s-tab-%d", id->get_ascii(), int(i)));

            MTEST_ASSERT(tab = new tk::Tab(tc->display()));
            MTEST_ASSERT(init_widget(tab, vh, full_id.get_utf8()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tab));
            MTEST_ASSERT(tc->add(tab) == STATUS_OK);
            tab->text()->set_raw(&tab_id);

            // Create Grid within tab
            MTEST_ASSERT(grid = new tk::Grid(tc->display()));
            MTEST_ASSERT(init_widget(grid, vh, "grid") == STATUS_OK);
            MTEST_ASSERT(widgets.push(grid));
            MTEST_ASSERT(tab->add(grid) == STATUS_OK);

            grid->rows()->set(3);
            grid->columns()->set(3);
            grid->orientation()->set_horizontal();
            grid->hspacing()->set(2);
            grid->vspacing()->set(2);

            // Fill grid with labels
            for (ssize_t y=0; y<3; ++y)
                for (ssize_t x=0; x<3; ++x)
                {
                    MTEST_ASSERT(full_id.fmt_ascii("%s\ntab-%d\nlabel-%d-%d", id->get_ascii(), int(i), x, y));
                    MTEST_ASSERT(label = new tk::Label(tc->display()));
                    MTEST_ASSERT(init_widget(label, vh, full_id.get_ascii()) == STATUS_OK);
                    MTEST_ASSERT(widgets.push(label));
                    MTEST_ASSERT(grid->add(label) == STATUS_OK);

                    label->text()->set_raw(&full_id);
                    label->text_layout()->set(x - 1, y - 1);
                }
        }

        return STATUS_OK;
    }

    MTEST_MAIN
    {
        lltl::parray<handler_t> vh;

        tk::Display *dpy = new tk::Display();
        MTEST_ASSERT(dpy != NULL);

        MTEST_ASSERT(dpy->init(0, NULL) == STATUS_OK);

        lltl::parray<tk::Widget> widgets;
        tk::Widget *w = NULL;
        tk::Window *wnd = new tk::Window(dpy);
        tk::Grid *grid = NULL;
        tk::TabControl *tc = NULL;

        // Initialize window
        MTEST_ASSERT(init_widget(wnd, vh, "window") == STATUS_OK);
        MTEST_ASSERT(wnd->title()->set_raw("Test tab control") == STATUS_OK);
        MTEST_ASSERT(wnd->role()->set_raw("tab_control_test") == STATUS_OK);
        wnd->bg_color()->set_rgb(0, 0.75, 1.0);
        wnd->actions()->set_actions(ws::WA_MOVE | ws::WA_RESIZE | ws::WA_CLOSE);
        wnd->border_style()->set(ws::BS_DIALOG);
        wnd->constraints()->set(160, 100, 640, 400);
        wnd->size()->set(320, 200);
        wnd->slot(tk::SLOT_CLOSE)->bind(slot_close, this);
        wnd->slot(tk::SLOT_KEY_UP)->bind(slot_key_up, this);
        wnd->pointer()->set(ws::MP_TABLE_CELL);
        wnd->layout()->set(-0.5, 0.5, 0.5, 0.5);
        MTEST_ASSERT(widgets.push(wnd));

        // Create grid
        MTEST_ASSERT(grid = new tk::Grid(dpy));
        MTEST_ASSERT(init_widget(grid, vh, "grid") == STATUS_OK);
        MTEST_ASSERT(widgets.push(grid));
        MTEST_ASSERT(wnd->add(grid) == STATUS_OK);
        grid->padding()->set_all(4);
        grid->hspacing()->set(8);
        grid->vspacing()->set(4);
        grid->bg_color()->set_rgb(1.0, 0.75, 1.0);
        grid->rows()->set(2);
        grid->columns()->set(3);

        {
            LSPString id;
            size_t gid = 0;

            // Row 1
            // Tab Control 1
            MTEST_ASSERT(id.fmt_ascii("tabcontrol-%d", int(gid++)));

            MTEST_ASSERT(tc = new tk::TabControl(dpy));
            MTEST_ASSERT(init_widget(tc, vh, id.get_ascii()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tc));
            MTEST_ASSERT(grid->add(tc) == STATUS_OK);
            MTEST_ASSERT(fill_tab_control(tc, widgets, vh, &id) == STATUS_OK);

            tc->heading()->set_halign(-1);
            tc->heading()->set_valign(-1);
            tc->heading_spacing()->set(-1);
            tc->tab_spacing()->set(0);

            // Tab Control 2
            MTEST_ASSERT(id.fmt_ascii("tabcontrol-%d", int(gid++)));

            MTEST_ASSERT(tc = new tk::TabControl(dpy));
            MTEST_ASSERT(init_widget(tc, vh, id.get_ascii()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tc));
            MTEST_ASSERT(grid->add(tc) == STATUS_OK);
            MTEST_ASSERT(fill_tab_control(tc, widgets, vh, &id) == STATUS_OK);

            tc->heading()->set_halign(0);
            tc->heading()->set_valign(-1);
            tc->heading_spacing()->set(0);
            tc->tab_spacing()->set(1);

            // Tab Control 3
            MTEST_ASSERT(id.fmt_ascii("tabcontrol-%d", int(gid++)));

            MTEST_ASSERT(tc = new tk::TabControl(dpy));
            MTEST_ASSERT(init_widget(tc, vh, id.get_ascii()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tc));
            MTEST_ASSERT(grid->add(tc) == STATUS_OK);
            MTEST_ASSERT(fill_tab_control(tc, widgets, vh, &id) == STATUS_OK);

            tc->heading()->set_halign(1);
            tc->heading()->set_valign(-1);
            tc->heading_spacing()->set(1);
            tc->tab_spacing()->set(2);
            tc->heading_gap()->set(2);
            tc->heading_color()->set("#008800");
            tc->heading_spacing_color()->set("#0088ff");
            tc->heading_gap_color()->set("#ffff00");
            tc->heading_fill()->set(true);
            tc->heading_spacing_fill()->set(true);

            // Row 2
            // Tab Control 4
            MTEST_ASSERT(id.fmt_ascii("tabcontrol-%d", int(gid++)));

            MTEST_ASSERT(tc = new tk::TabControl(dpy));
            MTEST_ASSERT(init_widget(tc, vh, id.get_ascii()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tc));
            MTEST_ASSERT(grid->add(tc) == STATUS_OK);
            MTEST_ASSERT(fill_tab_control(tc, widgets, vh, &id) == STATUS_OK);

            tc->heading()->set_halign(-1);
            tc->heading()->set_valign(1);
            tc->heading_spacing()->set(-1);
            tc->tab_spacing()->set(0);

            // Tab Control 5
            MTEST_ASSERT(id.fmt_ascii("tabcontrol-%d", int(gid++)));

            MTEST_ASSERT(tc = new tk::TabControl(dpy));
            MTEST_ASSERT(init_widget(tc, vh, id.get_ascii()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tc));
            MTEST_ASSERT(grid->add(tc) == STATUS_OK);
            MTEST_ASSERT(fill_tab_control(tc, widgets, vh, &id) == STATUS_OK);

            tc->heading()->set_halign(0);
            tc->heading()->set_valign(1);
            tc->heading_spacing()->set(0);
            tc->tab_spacing()->set(1);

            // Tab Control 6
            MTEST_ASSERT(id.fmt_ascii("tabcontrol-%d", int(gid++)));

            MTEST_ASSERT(tc = new tk::TabControl(dpy));
            MTEST_ASSERT(init_widget(tc, vh, id.get_ascii()) == STATUS_OK);
            MTEST_ASSERT(widgets.push(tc));
            MTEST_ASSERT(grid->add(tc) == STATUS_OK);
            MTEST_ASSERT(fill_tab_control(tc, widgets, vh, &id) == STATUS_OK);

            tc->heading()->set_halign(1);
            tc->heading()->set_valign(1);
            tc->heading_spacing()->set(2);
            tc->heading_gap()->set(2);
            tc->tab_spacing()->set(2);
            tc->heading_color()->set("#ffff00");
            tc->heading_spacing_color()->set("#ff0000");
            tc->heading_gap_color()->set("#0088ff");
            tc->heading_fill()->set(true);
            tc->heading_spacing_fill()->set(true);
        }

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
        destroy_handlers(vh);
    }

MTEST_END




