// Modified version of https://github.com/pfeerick/elapsedMillis

#pragma once

#include "Arduino.h"

class elapsedMillis
{
	private:
		unsigned long ms;
	public:
		elapsedMillis(void) { ms = millis(); }
		elapsedMillis(unsigned long val) { ms = millis() - val; }
		elapsedMillis(const elapsedMillis &orig) { ms = orig.ms; }
		operator unsigned long () const { return millis() - ms; }
		elapsedMillis & operator = (const elapsedMillis &rhs) { ms = rhs.ms; return *this; }
		elapsedMillis & operator = (unsigned long val) { ms = millis() - val; return *this; }
		elapsedMillis & operator -= (unsigned long val)      { ms += val ; return *this; }
		elapsedMillis & operator += (unsigned long val)      { ms -= val ; return *this; }
		elapsedMillis operator - (int val) const           { elapsedMillis r(*this); r.ms += val; return r; }
		elapsedMillis operator - (unsigned int val) const  { elapsedMillis r(*this); r.ms += val; return r; }
		elapsedMillis operator - (long val) const          { elapsedMillis r(*this); r.ms += val; return r; }
		elapsedMillis operator - (unsigned long val) const { elapsedMillis r(*this); r.ms += val; return r; }
		elapsedMillis operator + (int val) const           { elapsedMillis r(*this); r.ms -= val; return r; }
		elapsedMillis operator + (unsigned int val) const  { elapsedMillis r(*this); r.ms -= val; return r; }
		elapsedMillis operator + (long val) const          { elapsedMillis r(*this); r.ms -= val; return r; }
		elapsedMillis operator + (unsigned long val) const { elapsedMillis r(*this); r.ms -= val; return r; }
};
