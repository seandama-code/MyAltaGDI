//---------------------------------------------------------------------------
#ifndef ClassCasellaH
#define ClassCasellaH

#include <vector>
#include <boost/scoped_ptr.hpp>

#include "DefineGlobali.h"

//---------------------------------------------------------------------------
class TCasella
{
private:

    // le caselle sono 57

    int  idx_     ;  // identificativo (da 0 a 56)

    int  spigoloX_;
    int  spigoloY_;
    int  lato_    ;

    int  stato_   ;
    bool testata_ ;

    // -----------------------------------------
    // puntatori alle caselle vicine
    // si comincia da quella in alto a sx e si procede in senso orario
    int idxAltSin_;
    int idxAltCen_;
    int idxAltDes_;
    int idxDestra_;
    int idxBasDes_;
    int idxBasCen_;
    int idxBasSin_;
    int idxSinist_;

    // -----------------------------------------
    // funzioni private

public:
    TCasella();

    // -------- getter ---------------------------------
    int     GetIdx      () { return idx_      ; }

    int     GetSpigoloX () { return spigoloX_ ; }
    int     GetSpigoloY () { return spigoloY_ ; }

    int     GetStato    () { return stato_    ; }
    int     GetTestata  () { return testata_  ; }

    // -----------------------------------------
    // puntatori alle caselle vicine
    int     GetIdxAltSin() { return idxAltSin_; };
    int     GetIdxAltCen() { return idxAltCen_; };
    int     GetIdxAltDes() { return idxAltDes_; };
    int     GetIdxDestra() { return idxDestra_; };
    int     GetIdxBasDes() { return idxBasDes_; };
    int     GetIdxBasCen() { return idxBasCen_; };
    int     GetIdxBasSin() { return idxBasSin_; };
    int     GetIdxSinist() { return idxSinist_; };

    //  -------- setter ---------------------------------
    void    SetIdx      ( int  Val ) { idx_       = Val; }

    void    SetSpigoloX ( int  Val ) { spigoloX_  = Val; }
    void    SetSpigoloY ( int  Val ) { spigoloY_  = Val; }
    void    SetLato     ( int  Val ) { lato_      = Val; }

    void    SetStato    ( int  Val ) { stato_     = Val; }
    void    SetTestata  ( int  Val ) { testata_   = Val; }

    // -----------------------------------------
    // puntatori alle caselle vicine
    void    SetIdxAltSin( int  Val ) { idxAltSin_ = Val; };
    void    SetIdxAltCen( int  Val ) { idxAltCen_ = Val; };
    void    SetIdxAltDes( int  Val ) { idxAltDes_ = Val; };
    void    SetIdxDestra( int  Val ) { idxDestra_ = Val; };
    void    SetIdxBasDes( int  Val ) { idxBasDes_ = Val; };
    void    SetIdxBasCen( int  Val ) { idxBasCen_ = Val; };
    void    SetIdxBasSin( int  Val ) { idxBasSin_ = Val; };
    void    SetIdxSinist( int  Val ) { idxSinist_ = Val; };

    // -------- funzioni -------------------------------
    int     ClickSuCasella( int X, int Y );
};
//---------------------------------------------------------------------------
typedef   std::vector<TCasella>   CasellaVect;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

