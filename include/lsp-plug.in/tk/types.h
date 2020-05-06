/*
 * types.h
 *
 *  Created on: 5 мая 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TK_TYPES_H_
#define LSP_PLUG_IN_TK_TYPES_H_

#include <lsp-plug.in/tk/version.h>
#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/common/status.h>

namespace lsp
{
    namespace tk
    {
        /** Widget orientation
         *
         */
        enum orientation_t
        {
            O_HORIZONTAL,       //!< O_HORIZONTAL horizontal orientation
            O_VERTICAL          //!< O_VERTICAL vertical orientation
        };

        enum scrolling_t
        {
            SCROLL_NONE,        //!< SCROLL_NONE no scrolling permitted
            SCROLL_OPTIONAL,    //!< SCROLL_OPTIONAL scrolling is permitted but in optional state
            SCROLL_ALWAYS       //!< SCROLL_ALWAYS scrolling is permitted but in optional state
        };

        enum window_poilicy_t
        {
            WP_NORMAL,
            WP_GREEDY
        };

        typedef struct w_class_t
        {
            const char         *name;
            const w_class_t    *parent;
        } w_class_t;

        enum property_type_t
        {
            PT_INT,         // Integer property
            PT_FLOAT,       // Floating-point property
            PT_BOOL,        // Boolean property
            PT_STRING,      // String (text) property

            PT_UNKNOWN  = -1
        };

        typedef struct size_request_t
        {
            ssize_t             nMinWidth;
            ssize_t             nMinHeight;
            ssize_t             nMaxWidth;
            ssize_t             nMaxHeight;
        } size_request_t;

        typedef struct realize_t
        {
            ssize_t             nLeft;
            ssize_t             nTop;
            ssize_t             nWidth;
            ssize_t             nHeight;
        } realize_t;

        typedef struct padding_t
        {
            size_t              nLeft;
            size_t              nRight;
            size_t              nTop;
            size_t              nBottom;
        } padding_t;

        /**
         * Atom identifier
         */
        typedef ssize_t             atom_t;
    }
}

#endif /* LSP_PLUG_IN_TK_TYPES_H_ */
