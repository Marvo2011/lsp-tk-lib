/*
 * SlotSet.cpp
 *
 *  Created on: 12 июн. 2017 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/slots/SlotSet.h>

namespace lsp
{
    namespace tk
    {
        SlotSet::SlotSet()
        {
        }

        SlotSet::~SlotSet()
        {
            destroy();
        }

        void SlotSet::destroy()
        {
            size_t count        = vSlots.size();
            for (size_t i=0; i<count; ++i)
            {
                item_t *ptr     = vSlots.uget(i);
                if (ptr != NULL)
                    delete ptr;
            }
            vSlots.flush();
        }

        Slot *SlotSet::slot(slot_t id)
        {
            ssize_t first   = 0, last = ssize_t(vSlots.size()) - 1;

            while (first <= last)
            {
                ssize_t idx     = (first + last) >> 1;
                item_t *ptr     = vSlots.uget(idx);
                if (ptr->nType == id)
                    return &ptr->sSlot;
                else if (ptr->nType < id)
                    first       = idx + 1;
                else
                    last        = idx - 1;
            }

            return NULL;
        }

        Slot *SlotSet::add(slot_t id)
        {
            // Find position to insert slot
            item_t *ptr;
            ssize_t first   = 0, last = ssize_t(vSlots.size()) - 1, idx = 0;

            while (first <= last)
            {
                idx             = (first + last) >> 1;
                ptr             = vSlots.uget(idx);
                if (ptr->nType == id)
                    return &ptr->sSlot;
                else if (ptr->nType < id)
                    first       = ++idx;
                else
                    last        = --idx;
            }

            // Now allocate new slot
            if ((ptr = new item_t) == NULL)
                return NULL;
            ptr->nType          = id;

            // Add slot to sorted list
            if (!vSlots.insert(first, ptr))
            {
                delete ptr;
                return NULL;
            }

            return &ptr->sSlot;
        }
    
        handler_id_t SlotSet::add(slot_t id, event_handler_t handler, void *arg, bool enabled)
        {
            // Check data
            if (handler == NULL)
                return - STATUS_BAD_ARGUMENTS;

            // Find position to insert slot
            item_t *ptr;
            ssize_t first   = 0, last = ssize_t(vSlots.size()) - 1, idx = 0;

            while (first <= last)
            {
                idx             = (first + last) >> 1;
                item_t *ptr     = vSlots.uget(idx);
                if (ptr->nType == id)
                    return ptr->sSlot.bind(handler, arg, enabled);
                else if (ptr->nType < id)
                    first       = ++idx;
                else
                    last        = --idx;
            }

            // Now allocate new slot
            if ((ptr = new item_t) == NULL)
                return NULL;
            ptr->nType          = id;

            // Bind data to slot
            handler_id_t hid    = ptr->sSlot.bind(handler, arg, enabled);
            if (hid < 0)
            {
                delete ptr;
                return hid;
            }

            // Add slot to sorted list
            if (!vSlots.insert(first, ptr))
            {
                delete ptr;
                return - STATUS_NO_MEM;
            }

            return hid;
        }

        handler_id_t SlotSet::bind(slot_t id, event_handler_t handler, void *arg, bool enabled)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? - STATUS_NOT_FOUND : s->bind(handler, arg, enabled);
        }

        handler_id_t SlotSet::intercept(slot_t id, event_handler_t handler, void *arg, bool enabled)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? - STATUS_NOT_FOUND : s->intercept(handler, arg, enabled);
        }

        status_t SlotSet::unbind(slot_t id, handler_id_t handler)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? STATUS_NOT_FOUND : s->unbind(handler);
        }

        handler_id_t SlotSet::unbind(slot_t id, event_handler_t handler, void *arg)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? - STATUS_NOT_FOUND : s->unbind(handler, arg);
        }

        size_t SlotSet::unbind_all(slot_t id)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? 0 : s->unbind_all();
        }

        status_t SlotSet::disable(slot_t id, handler_id_t handler)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? STATUS_NOT_FOUND : s->disable(handler);
        }

        ssize_t SlotSet::disable_all(slot_t id)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? 0 : s->disable_all();
        }

        status_t SlotSet::enable(slot_t id, handler_id_t handler)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? STATUS_NOT_FOUND : s->enable(handler);
        }

        size_t SlotSet::enable_all(slot_t id)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? 0 : s->enable_all();
        }

        status_t SlotSet::execute(slot_t id, Widget *sender, void *data)
        {
            Slot *s         = slot(id);
            return (s == NULL) ? STATUS_NOT_FOUND : s->execute(sender, data);
        }
    }

} /* namespace lsp */
