/*
 * Schema.h
 *
 *  Created on: 6 мая 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TK_STYLE_SCHEMA_H_
#define LSP_PLUG_IN_TK_STYLE_SCHEMA_H_

#ifndef LSP_PLUG_IN_TK_IMPL
    #error "use <lsp-plug.in/tk/tk.h>"
#endif

#include <lsp-plug.in/lltl/parray.h>
#include <lsp-plug.in/lltl/pphash.h>
#include <lsp-plug.in/runtime/LSPString.h>
#include <lsp-plug.in/runtime/Color.h>
#include <lsp-plug.in/io/Path.h>
#include <lsp-plug.in/io/IInStream.h>
#include <lsp-plug.in/io/IInSequence.h>
#include <lsp-plug.in/fmt/xml/PullParser.h>

namespace lsp
{
    namespace tk
    {
        class Display;

        class Schema
        {
            private:
                Schema & operator = (const Schema &);

            protected:
                typedef struct style_t
                {
                    Style                       sStyle;
                    lltl::parray<LSPString>     vParents;
                    bool                        bInitialized;
                } style_t;

                typedef struct context_t
                {
                    lltl::pphash<LSPString, lsp::Color> vColors;    // Color aliases
                    lltl::pphash<LSPString, style_t>    vStyles;    // Styles
                    style_t                            *pRoot;      // Root style
                } context_t;

                typedef struct property_value_t
                {
                    property_type_t     type;
                    union
                    {
                        bool            bvalue;
                        int             ivalue;
                        float           fvalue;
                    };
                    LSPString           svalue;
                } property_value_t;

            protected:
                context_t           sCtx;
                Display            *pDisplay;

            protected:
                status_t            apply_context(context_t *ctx);
                static void         init_context(context_t *ctx);
                static void         destroy_context(context_t *ctx);
                static void         destroy_style(style_t *style);

                status_t            parse_document(xml::PullParser *p);
                static status_t     parse_schema(xml::PullParser *p, context_t *ctx);
                static status_t     parse_colors(xml::PullParser *p, context_t *ctx);
                static status_t     parse_style(xml::PullParser *p, context_t *ctx, bool root);
                static status_t     parse_color(xml::PullParser *p, lsp::Color *color);
                static status_t     parse_property(xml::PullParser *p, style_t *style, const LSPString *name);

                static status_t     parse_style_class(LSPString *cname, const LSPString *text);
                static status_t     parse_style_parents(style_t *style, const LSPString *text);
                static status_t     parse_property_type(property_type_t *pt, const LSPString *text);
                static status_t     parse_property_value(property_value_t *v, const LSPString *text, property_type_t pt);

            public:
                explicit Schema(Display *dpy);
                virtual ~Schema();

            public:
                status_t            parse_file(const char *path, const char *charset = NULL);
                status_t            parse_file(const LSPString *path, const char *charset = NULL);
                status_t            parse_file(const io::Path *path, const char *charset = NULL);

                status_t            parse_data(io::IInStream *is, size_t flags = WRAP_NONE, const char *charset = NULL);
                status_t            parse_data(const char *str, const char *charset = NULL);
                status_t            parse_data(const LSPString *str);
                status_t            parse_data(io::IInSequence *seq, size_t flags = WRAP_NONE);
        };
    
    } /* namespace tk */
} /* namespace lsp */

#endif /* LSP_PLUG_IN_TK_STYLE_SCHEMA_H_ */
