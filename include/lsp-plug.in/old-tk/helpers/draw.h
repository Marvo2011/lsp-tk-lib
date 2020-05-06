/*
 * draw.h
 *
 *  Created on: 29 окт. 2017 г.
 *      Author: sadko
 */

#ifndef UI_TK_HELPERS_DRAW_H_
#define UI_TK_HELPERS_DRAW_H_

#include <lsp-plug.in/tk-old/types.h>
#include <lsp-plug.in/tk-old/version.h>
#include <lsp-plug.in/ws/ISurface.h>

namespace lsp
{
    namespace tk
    {
        /** Draw border
         *
         * @param s surface to draw the border
         * @param left the left position
         * @param top the top position
         * @param width the width of the border
         * @param height the height of the border
         * @param thick the thickness of the border
         * @param iradius inner radius of the border
         * @param mask the rounding mask
         * @param c the color of the border
         */
        void draw_border(ws::ISurface *s, ssize_t left, ssize_t top, ssize_t width, ssize_t height, ssize_t thick, size_t iradius, size_t mask, const Color &c);

        /** Create glass
         *
         * @param s the factory surface
         * @param g pointer to pointer that stores address of the surface object
         * @param width the width of the glass
         * @param height the height of the glass
         * @param radius the radius of the glass
         * @param mask the radius drawing mask
         * @return pointer to the glass on succes or null on error
         */
        ws::ISurface *create_glass(ws::ISurface *s, ws::ISurface **g, size_t width, size_t height, size_t radius, size_t mask);

        /** Create glass with border
         *
         * @param s the factory surface
         * @param g pointer to pointer that stores address of the surface object
         * @param width the width of the glass
         * @param height the height of the glass
         * @param radius the radius of the glass
         * @param mask the radius drawing mask
         * @param c the color of the border
         * @return pointer to the glass on succes or null on error
         */
        ws::ISurface * create_border_glass(
            ws::ISurface *s, ws::ISurface **g,
            size_t width, size_t height,
            ssize_t thick, size_t radius,
            size_t mask, const Color &c);

        void draw_border_back(
            ws::ISurface *s,
            ssize_t left, ssize_t top,
            size_t width, size_t height,
            ssize_t thick, size_t radius,
            size_t mask, const Color &c
        );
    }
}

#endif /* UI_TK_HELPERS_DRAW_H_ */
