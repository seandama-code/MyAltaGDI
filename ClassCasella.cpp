//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DefineGlobali.h"

#include "ClassCasella.h"

//---------------------------------------------------------------------------

TCasella::TCasella()
{
    // le caselle sono 57
    idx_      =  0;  // identificativo (da 0 a 56)

    spigoloX_ =  0;
    spigoloY_ =  0;
    lato_     = 74;

    stato_    = LIBERO;

    // -----------------------------------------
    // puntatori alle caselle vicine
    idxAltSin_ = NO_VAL;
    idxAltCen_ = NO_VAL;
    idxAltDes_ = NO_VAL;
    idxDestra_ = NO_VAL;
    idxBasDes_ = NO_VAL;
    idxBasCen_ = NO_VAL;
    idxBasSin_ = NO_VAL;
    idxSinist_ = NO_VAL;

}
//---------------------------------------------------------------------------

int TCasella::ClickSuCasella( int X, int Y )
{
    int x1 = spigoloX_;
    int y1 = spigoloY_;
    int x2 = spigoloX_ + lato_;
    int y2 = spigoloY_ + lato_;

    if( Y > y1 && Y < y2 && X > x1 && X < x2 )
    {
        return idx_;
    }

    return NO_VAL;
}
//---------------------------------------------------------------------------

