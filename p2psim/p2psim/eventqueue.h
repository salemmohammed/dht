/*
 * Copyright (c) 2003 Thomer M. Gil (thomer@csail.mit.edu)
 *                    Robert Morris (rtm@csail.mit.edu)
 *                    Massachusetts Institute of Technology
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 */

#ifndef __EVENTQUEUE_H
#define __EVENTQUEUE_H

#include "event.h"
#include "observed.h"
#include "../utils/skiplist.h"
using namespace std;

class EventQueue : public Threaded, public Observed {
  friend class EventQueueObserver;

  // XXX: remove this
  // are executed in EventQueue's thread, so allowed.
  friend class Oldobserver;

public:
  static EventQueue* Instance();
  void here(Event *e);
  Time time() { return _time; }
  void go();

private:
  EventQueue();
  ~EventQueue();

  struct eq_entry {
    eq_entry() { ts = 0; events.clear(); }
    eq_entry(Event *e) { ts = e->ts; events.clear(); }
    Time ts;
    vector<Event*> events;
    sklist_entry<eq_entry> _sortlink;
  };
  skiplist<eq_entry, Time, &eq_entry::ts, &eq_entry::_sortlink> _queue;

  static EventQueue *_instance;
  Time _time;
  Channel *_eventchan;
  Channel *_gochan;

  virtual void run();
  void add_event(Event*);
  bool advance();
  Channel* eventchan() { return _eventchan; }

  // for debuging
  void dump();
};
 
#endif // __EVENTQUEUE_H
