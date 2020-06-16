/*
 * Rectangle.cpp
 *
 *  Created on: 16 июн. 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/tk.h>

namespace lsp
{
    namespace tk
    {
        const prop::desc_t Rectangle::DESC[] =
        {
            { "",               PT_STRING   },
            { ".left",          PT_INT      },
            { ".top",           PT_INT      },
            { ".width",         PT_INT      },
            { ".height",        PT_INT      },
            { NULL,             PT_UNKNOWN  }
        };

        void Rectangle::Listener::notify(atom_t property)
        {
            pValue->commit(property);
        }

        Rectangle::Rectangle(prop::Listener *listener):
            MultiProperty(listener),
            sListener(this)
        {
            sRect.nLeft     = 0;
            sRect.nTop      = 0;
            sRect.nWidth    = 0;
            sRect.nHeight   = 0;
        }

        Rectangle::~Rectangle()
        {
            MultiProperty::unbind(vAtoms, DESC, &sListener);
        }

        void Rectangle::commit(atom_t property)
        {
            if ((pStyle == NULL) || (property < 0))
                return;

            LSPString s;
            ssize_t v;
            if ((property == vAtoms[P_LEFT]) && (pStyle->get_int(vAtoms[P_LEFT], &v) == STATUS_OK))
                sRect.nLeft     = v;
            if ((property == vAtoms[P_TOP]) && (pStyle->get_int(vAtoms[P_TOP], &v) == STATUS_OK))
                sRect.nTop      = v;
            if ((property == vAtoms[P_WIDTH]) && (pStyle->get_int(vAtoms[P_WIDTH], &v) == STATUS_OK))
                sRect.nWidth    = lsp_max(0, v);
            if ((property == vAtoms[P_HEIGHT]) && (pStyle->get_int(vAtoms[P_HEIGHT], &v) == STATUS_OK))
                sRect.nHeight   = lsp_max(0, v);

            // Compound property
            if ((property == vAtoms[P_VALUE]) && (pStyle->get_string(vAtoms[P_VALUE], &s) == STATUS_OK))
            {
                ssize_t v[4];
                size_t n    = Property::parse_ints(v, 4, &s);
                switch (n)
                {
                    case 2:
                        sRect.nLeft     = 0;
                        sRect.nTop      = 0;
                        sRect.nWidth    = lsp_max(0, v[0]);
                        sRect.nHeight   = lsp_max(0, v[1]);
                        break;
                    case 4:
                        sRect.nLeft     = v[0];
                        sRect.nTop      = v[1];
                        sRect.nWidth    = lsp_max(0, v[2]);
                        sRect.nHeight   = lsp_max(0, v[3]);
                        break;
                    default:
                        break;
                }
            }
            if (pListener != NULL)
                pListener->notify(this);
        }

        void Rectangle::sync()
        {
            if (pStyle != NULL)
            {
                pStyle->begin(&sListener);
                {
                    LSPString s;

                    // Simple components
                    if (vAtoms[P_LEFT] >= 0)
                        pStyle->set_int(vAtoms[P_LEFT], sRect.nLeft);
                    if (vAtoms[P_TOP] >= 0)
                        pStyle->set_int(vAtoms[P_TOP], sRect.nTop);
                    if (vAtoms[P_WIDTH] >= 0)
                        pStyle->set_int(vAtoms[P_WIDTH], sRect.nWidth);
                    if (vAtoms[P_HEIGHT] >= 0)
                        pStyle->set_int(vAtoms[P_HEIGHT], sRect.nHeight);

                    // Compound properties
                    s.fmt_ascii("%ld %ld %ld %ld", long(sRect.nLeft), long(sRect.nTop), long(sRect.nWidth), long(sRect.nHeight));
                    if (vAtoms[P_VALUE] >= 0)
                        pStyle->set_string(vAtoms[P_VALUE], &s);
                }
                pStyle->end();
            }
            if (pListener != NULL)
                pListener->notify(this);
        }

        ssize_t Rectangle::set_left(ssize_t v)
        {
            ssize_t old     = sRect.nLeft;
            if (old == v)
                return old;
            sRect.nLeft     = v;
            sync();
            return old;
        }

        ssize_t Rectangle::set_top(ssize_t v)
        {
            ssize_t old     = sRect.nTop;
            if (old == v)
                return old;
            sRect.nTop      = v;
            sync();
            return old;
        }

        ssize_t Rectangle::set_width(ssize_t v)
        {
            ssize_t old     = sRect.nWidth;
            if (old == v)
                return old;
            sRect.nWidth    = v;
            sync();
            return old;
        }

        ssize_t Rectangle::set_height(ssize_t v)
        {
            ssize_t old     = sRect.nHeight;
            if (old == v)
                return old;
            sRect.nHeight   = v;
            sync();
            return old;
        }

        void Rectangle::set_position(ssize_t left, ssize_t top)
        {
            if ((sRect.nLeft == left) &&
                (sRect.nTop == top))
                return;
            sRect.nLeft     = left;
            sRect.nTop      = top;
            sync();
        }

        void Rectangle::set_size(ssize_t width, ssize_t height)
        {
            width   = lsp_max(0, width);
            height  = lsp_max(0, height);

            if ((sRect.nWidth == width) &&
                (sRect.nHeight == height))
                return;

            sRect.nWidth    = width;
            sRect.nHeight   = height;
            sync();
        }

        void Rectangle::set(ssize_t left, ssize_t top, ssize_t width, ssize_t height)
        {
            width   = lsp_max(0, width);
            height  = lsp_max(0, height);

            if ((sRect.nLeft == left) &&
                (sRect.nTop == top) &&
                (sRect.nWidth == width) &&
                (sRect.nHeight == height))
                return;

            sRect.nLeft     = left;
            sRect.nTop      = top;
            sRect.nWidth    = width;
            sRect.nHeight   = height;
            sync();
        }

        void Rectangle::set(const ws::rectangle_t *r)
        {
            ssize_t width   = lsp_max(0, r->nWidth);
            ssize_t height  = lsp_max(0, r->nHeight);

            if ((sRect.nLeft == r->nLeft) &&
                (sRect.nTop == r->nTop) &&
                (sRect.nWidth == width) &&
                (sRect.nHeight == height))
                return;

            sRect.nLeft     = r->nLeft;
            sRect.nTop      = r->nTop;
            sRect.nWidth    = width;
            sRect.nHeight   = height;
            sync();
        }

        namespace prop
        {
            status_t Rectangle::init(Style *style)
            {
                ws::rectangle_t r;
                r.nLeft     = 0;
                r.nTop      = 0;
                r.nWidth    = 0;
                r.nHeight   = 0;

                return init(style, &r);
            }

            status_t Rectangle::init(Style *style, ssize_t left, ssize_t top, ssize_t width, ssize_t height)
            {
                ws::rectangle_t r;
                r.nLeft     = left;
                r.nTop      = top;
                r.nWidth    = width;
                r.nHeight   = height;

                return init(style, &r);
            }

            status_t Rectangle::init(Style *style, const ws::rectangle_t *rect)
            {
                if (pStyle == NULL)
                    return STATUS_BAD_STATE;

                style->begin();
                {
                    LSPString s;

                    style->create_int(vAtoms[P_LEFT], rect->nLeft);
                    style->create_int(vAtoms[P_TOP], rect->nTop);
                    style->create_int(vAtoms[P_WIDTH], rect->nWidth);
                    style->create_int(vAtoms[P_HEIGHT], rect->nHeight);

                    // Compound properties
                    s.fmt_ascii("%ld %ld %ld %ld", long(rect->nLeft), long(rect->nTop), long(rect->nWidth), long(rect->nHeight));
                    pStyle->set_string(vAtoms[P_VALUE], &s);
                }
                style->end();
                return STATUS_OK;
            }

            status_t Rectangle::override(Style *style)
            {
                ws::rectangle_t r;
                r.nLeft     = 0;
                r.nTop      = 0;
                r.nWidth    = 0;
                r.nHeight   = 0;

                return init(style, &r);
            }

            status_t Rectangle::override(Style *style, ssize_t left, ssize_t top, ssize_t width, ssize_t height)
            {
                ws::rectangle_t r;
                r.nLeft     = left;
                r.nTop      = top;
                r.nWidth    = width;
                r.nHeight   = height;

                return init(style, &r);
            }

            status_t Rectangle::override(Style *style, const ws::rectangle_t *rect)
            {
                if (pStyle == NULL)
                    return STATUS_BAD_STATE;

                style->begin();
                {
                    LSPString s;

                    style->override_int(vAtoms[P_LEFT], rect->nLeft);
                    style->override_int(vAtoms[P_TOP], rect->nTop);
                    style->override_int(vAtoms[P_WIDTH], rect->nWidth);
                    style->override_int(vAtoms[P_HEIGHT], rect->nHeight);

                    // Compound properties
                    s.fmt_ascii("%ld %ld %ld %ld", long(rect->nLeft), long(rect->nTop), long(rect->nWidth), long(rect->nHeight));
                    pStyle->set_string(vAtoms[P_VALUE], &s);
                }
                style->end();
                return STATUS_OK;
            }
        }
    } /* namespace tk */
} /* namespace lsp */





