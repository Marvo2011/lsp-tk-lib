/*
 * GraphMarker.h
 *
 *  Created on: 20 авг. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TK_WIDGETS_GRAPH_GRAPHMARKER_H_
#define LSP_PLUG_IN_TK_WIDGETS_GRAPH_GRAPHMARKER_H_

#ifndef LSP_PLUG_IN_TK_IMPL
    #error "use <lsp-plug.in/tk/tk.h>"
#endif

namespace lsp
{
    namespace tk
    {
        class GraphMarker: public GraphItem
        {
            public:
                static const w_class_t    metadata;

            private:
                GraphMarker & operator = (const GraphMarker &);

            protected:
                enum flags_t
                {
                    F_HIGHLIGHT     = 1 << 0,
                    F_EDITING       = 1 << 1,
                    F_FINE_TUNE     = 1 << 2
                };

            protected:
                prop::Integer               sOrigin;        // Origin
                prop::Integer               sBasis;         // Index of basis axis
                prop::Integer               sParallel;      // Index of parallel axis
                prop::Float                 sValue;         // Actual value
                prop::Float                 sMin;           // Minimum possible value when editing
                prop::Float                 sMax;           // Minimum possible value when editing
                prop::Vector2D              sDirection;     // Direction
                prop::Integer               sWidth;         // Thickness
                prop::Integer               sHWidth;        // Hover width
                prop::Boolean               sEditable;      // Editable flag
                prop::Integer               sBorder;        // Highlighting border
                prop::Color                 sColor;         // Color
                prop::Color                 sHColor;        // Selected Color
                prop::Color                 sBorderColor;   // Border Color

                float                       fOffset;        // Offset
                size_t                      nXFlags;        // Extra flags

            protected:
                virtual void                property_changed(Property *prop);

            public:
                explicit GraphMarker(Display *dpy);
                virtual ~GraphMarker();

                virtual status_t            init();

            public:
                LSP_TK_PROPERTY(Integer,            origin,                 &sOrigin);
                LSP_TK_PROPERTY(Integer,            basis,                  &sBasis);
                LSP_TK_PROPERTY(Integer,            parallel,               &sParallel);
                LSP_TK_PROPERTY(Float,              value,                  &sValue);
                LSP_TK_PROPERTY(Float,              min,                    &sMin);
                LSP_TK_PROPERTY(Float,              max,                    &sMax);
                LSP_TK_PROPERTY(Vector2D,           direction,              &sDirection);
                LSP_TK_PROPERTY(Integer,            width,                  &sWidth);
                LSP_TK_PROPERTY(Integer,            hover_width,            &sHWidth);
                LSP_TK_PROPERTY(Boolean,            editable,               &sEditable);
                LSP_TK_PROPERTY(Integer,            border,                 &sBorder);
                LSP_TK_PROPERTY(Color,              color,                  &sColor);
                LSP_TK_PROPERTY(Color,              hover_color,            &sHColor);
                LSP_TK_PROPERTY(Color,              border_color,           &sBorderColor);

            public:
                virtual void                render(ws::ISurface *s, const ws::rectangle_t *area, bool force);
        };
    }
}


#endif /* LSP_PLUG_IN_TK_WIDGETS_GRAPH_GRAPHMARKER_H_ */
