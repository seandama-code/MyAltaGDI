//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <tchar.h>
#include <algorithm>

// serve per la funzione: sndPlaySound ("xx.wav", SND_ASYNC | SND_FILENAME);
#include <mmsystem.h>
#include "AdditionalWav.h"
#include "AdditionalRes.h"

#pragma option push
#pragma warn -8022      // W8022 'func1' hides virtual function 'func2'
#include "GdiPlusUtils.h"
#pragma option pop

#include "GdiplusGraphics.h"
#include "GdiPlus.h"

#include "DefineGlobali.h"

#include "FormRegole.h"

#include "ClassCasella.h"

#include "FormMain.h"

using boost::scoped_ptr;

using Gdiplus::SolidBrush;
using Gdiplus::Pen;
using Gdiplus::RectF;

using Gdiplus::PointF;
//using Gdiplus::Color;
using Gdiplus::LinearGradientBrush;
using Gdiplus::PathGradientBrush;
using Gdiplus::GraphicsPath;

using GdiplusUtils::GdiPlusSessionManager;
using GdiplusUtils::TColorToGdiplusColor;
using GdiplusUtils::VCLRectToGdiPlusRectF;
using GdiplusUtils::DrawImage;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
    : TForm(Owner)
{
    // ===========================================================
    // inizializzazione delle GDI
    #pragma option push
    #pragma warn -8104      //Local Static with constructor dangerous for multi-threaded apps
    static GdiPlusSessionManager volatile GdiPlusSessionManagerInstance;
    #pragma option pop

    ListBox1->Visible           = false;

    playerWin_                  = NO_VAL;

    LabelTitolo->Transparent    = true ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
    Width  = 916;
    Height = 878+40;

    larBoard_    = PaintBoxBoard->Width ;
	altBoard_    = PaintBoxBoard->Height;

    player_      = PLAYER;

    RadioGroupAvversario->Visible = false;

    // =======================================================
    String Res = RES_ZZ_FONDO;
    bmpBoard_  =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );

    Res = RES_ZZ_PEDDIAGSIN;
    bmpPedDiagSin_ =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );

    Res = RES_ZZ_PEDDIAGDES;
    bmpPedDiagDes_ =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );

    // =======================================================
    Res = RES_ZZ_LEDVERDE;
    bmpLedVerde_ =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );
    Res = RES_ZZ_LEDROSSO;
    bmpLedRosso_ =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );
    Res = RES_ZZ_LEDSPENT;
    bmpLedSpent_ =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );

    // =======================================================
    bmpBaseGhost_  = new Gdiplus::Bitmap  ( larBoard_, altBoard_ );
    gdiBaseGhost_  = new Gdiplus::Graphics( bmpBaseGhost_ );

    bmpGhost_      = new Gdiplus::Bitmap  ( larBoard_, altBoard_ );
    gdiGhost_      = new Gdiplus::Graphics( bmpGhost_ );

    // =======================================================
    spigoloPedSinX_ =  40;
    spigoloPedSinY_ = 680;
    spigoloPedDesX_ = 780;
    spigoloPedDesY_ = 680;

    // =======================================================
    lato_       = 74;
    latoPedina_ = 40;
    CreaCaselle();

    BitBtnNuovaPartitaClick( 0 );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    if( bmpBoard_  ) delete bmpBoard_ ;

    if( bmpLedVerde_  ) delete bmpLedVerde_ ;
    if( bmpLedRosso_  ) delete bmpLedRosso_ ;
    if( bmpLedSpent_  ) delete bmpLedSpent_ ;

    // ---------------------------------------------
    if( bmpBaseGhost_ ) delete bmpBaseGhost_;
    if( gdiBaseGhost_ ) delete gdiBaseGhost_;

    if( bmpGhost_     ) delete bmpGhost_    ;
    if( gdiGhost_     ) delete gdiGhost_    ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ButtonRegoleClick(TObject *Sender)
{
    scoped_ptr<TfrmRegole> frm( new TfrmRegole( 0 ) );

    frm->ShowModal ();

    if( frm->Tag )
    {
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PaintBoxBoardPaint(TObject *Sender)
{
    DisegnaSituazione( true );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::BitBtnNuovaPartitaClick(TObject *Sender)
{
    player_      = PLAYER;
    playerWin_   = NO_VAL;

    partitaFinita_ = false;

	ListBox1->Visible = false;
	ListBox1->Clear();

	ListBox2->Visible = false;
	ListBox2->Clear();

	ListBox3->Visible = false;
	ListBox3->Clear();

	// -------------------------------------------
    // prima setto tutti le caselle a LIBERO
    for( UINT i = 0; i < caselleVect_.size(); i ++ )
        caselleVect_[i].SetStato( LIBERO );



caselleVect_[24].SetStato( DIAGSIN );
caselleVect_[25].SetStato( DIAGDES );
caselleVect_[26].SetStato( DIAGSIN );
caselleVect_[27].SetStato( DIAGDES );
//caselleVect_[28].SetStato( DIAGSIN );
caselleVect_[29].SetStato( DIAGDES );
caselleVect_[30].SetStato( DIAGSIN );

caselleVect_[31].SetStato( DIAGDES );
caselleVect_[32].SetStato( DIAGSIN );

caselleVect_[ 1].SetStato( DIAGSIN );
caselleVect_[ 5].SetStato( DIAGDES );
caselleVect_[11].SetStato( DIAGSIN );
caselleVect_[18].SetStato( DIAGSIN );
caselleVect_[19].SetStato( DIAGDES );

caselleVect_[37].SetStato( DIAGDES );
caselleVect_[45].SetStato( DIAGSIN );
caselleVect_[51].SetStato( DIAGDES );
caselleVect_[55].SetStato( DIAGSIN );


    // -------------------------------------------
    DisegnaSituazione( true );
}
//---------------------------------------------------------------------------

void TfrmMain::CreaCaselle()
{
    int Num[9] = { 3, 5, 7, 9, 9, 9, 7, 5, 3 };

    int StartX = START_X;
    int StartY = START_Y;

    // le caselle sono 57
    int Idx = 0;
    for( int j = 0; j < 9; j ++ )            // righe
    {
        int Dx = 0;
        if( j == 0 || j == 8 ) Dx += lato_ * 3;
        if( j == 1 || j == 7 ) Dx += lato_ * 2;
        if( j == 2 || j == 6 ) Dx += lato_ * 1;

        for( int i = 0; i < Num[j]; i ++ )   // colonne
        {
            TCasella C;

            C.SetIdx     ( Idx ++ );

            C.SetSpigoloX( StartX + Dx );
            C.SetSpigoloY( StartY );
            C.SetLato    ( lato_ );

            C.SetStato( LIBERO  );

            caselleVect_.push_back( C );

            Dx += lato_;
        }

        StartY += lato_;
    }

    // ==================================================
    // inserisco gli Idx delle caselle contigue
    // si comincia da quella in alto a sx e si procede in senso orario
    int Ptr[57][8] = {   { NV, NV, NV,  1,  5,  4,  3, NV },  //  0
                         { NV, NV, NV,  2,  6,  5,  4,  0 },  //  1
                         { NV, NV, NV, NV,  7,  6,  5,  1 },  //  2

                         { NV, NV,  0,  4, 10,  9,  8, NV },  //  3
                         { NV,  0,  1,  5, 11, 10,  9,  3 },  //  4
                         {  0,  1,  2,  6, 12, 11, 10,  4 },  //  5
                         {  1,  2, NV,  7, 13, 12, 11,  5 },  //  6
                         {  2, NV, NV, NV, 14, 13, 12,  6 },  //  7

                         { NV, NV,  3,  9, 17, 16, 15, NV },  //  8
                         { NV,  3,  4, 10, 18, 17, 16,  8 },  //  9
                         {  3,  4,  5, 11, 19, 18, 17,  9 },  // 10
                         {  4,  5,  6, 12, 20, 19, 18, 10 },  // 11
                         {  5,  6,  7, 13, 21, 20, 19, 11 },  // 12
                         {  6,  7, NV, 14, 22, 21, 20, 12 },  // 13
                         {  7, NV, NV, NV, 23, 22, 21, 13 },  // 14

                         { NV, NV,  8, 16, 25, 24, NV, NV },  // 15
                         { NV,  8,  9, 17, 26, 25, 24, 15 },  // 16
                         {  8,  9, 10, 18, 27, 26, 25, 16 },  // 17
                         {  9, 10, 11, 19, 28, 27, 26, 17 },  // 18
                         { 10, 11, 12, 20, 29, 28, 27, 18 },  // 19
                         { 11, 12, 13, 21, 30, 29, 28, 19 },  // 20
                         { 12, 13, 14, 22, 31, 30, 29, 20 },  // 21
                         { 13, 14, NV, 23, 32, 31, 30, 21 },  // 22
                         { 14, NV, NV, NV, NV, 32, 31, 22 },  // 23

                         { NV, 15, 16, 25, 34, 33, NV, NV },  // 24
                         { 15, 16, 17, 26, 35, 34, 33, 24 },  // 25
                         { 16, 17, 18, 27, 36, 35, 34, 25 },  // 26
                         { 17, 18, 19, 28, 37, 36, 35, 26 },  // 27
                         { 18, 19, 20, 29, 38, 37, 36, 27 },  // 28
                         { 19, 20, 21, 30, 39, 38, 37, 28 },  // 29
                         { 20, 21, 22, 31, 40, 39, 38, 29 },  // 30
                         { 21, 22, 23, 32, 41, 40, 39, 30 },  // 31
                         { 22, 23, NV, NV, NV, 41, 40, 31 },  // 32

                         { NV, 24, 25, 34, 42, NV, NV, NV },  // 33
                         { 24, 25, 26, 35, 43, 42, NV, 33 },  // 34
                         { 25, 26, 27, 36, 44, 43, 42, 34 },  // 35
                         { 26, 27, 28, 37, 45, 44, 43, 35 },  // 36
                         { 27, 28, 29, 38, 46, 45, 44, 36 },  // 37
                         { 28, 29, 30, 39, 47, 46, 45, 37 },  // 38
                         { 29, 30, 31, 40, 48, 47, 46, 38 },  // 39
                         { 30, 31, 32, 41, NV, 48, 47, 39 },  // 40
                         { 31, 32, NV, NV, NV, NV, 48, 40 },  // 41

                         { 33, 34, 35, 43, 49, NV, NV, NV },  // 42
                         { 34, 35, 36, 44, 50, 49, NV, 42 },  // 43
                         { 35, 36, 37, 45, 51, 50, 49, 43 },  // 44
                         { 36, 37, 38, 46, 52, 51, 50, 44 },  // 45
                         { 37, 38, 39, 47, 53, 52, 51, 45 },  // 46
                         { 38, 39, 40, 48, NV, 53, 52, 46 },  // 47
                         { 39, 40, 41, NV, NV, NV, 53, 47 },  // 48

                         { 42, 43, 44, 50, 54, NV, NV, NV },  // 49
                         { 43, 44, 45, 51, 55, 54, NV, 49 },  // 50
                         { 44, 45, 46, 52, 56, 55, 54, 50 },  // 51
                         { 45, 46, 47, 53, NV, NV, NV, 51 },  // 52
                         { 46, 47, 48, NV, NV, NV, 56, 52 },  // 53

                         { 49, 50, 51, 55, NV, NV, NV, NV },  // 54
                         { 50, 51, 52, 56, NV, NV, NV, 54 },  // 55
                         { 51, 52, 53, NV, NV, NV, NV, 55 }   // 56
                     };

    for( UINT i = 0; i < caselleVect_.size(); i ++ )
    {
        caselleVect_[i].SetIdxAltSin( Ptr[i][0] );
        caselleVect_[i].SetIdxAltCen( Ptr[i][1] );
        caselleVect_[i].SetIdxAltDes( Ptr[i][2] );
        caselleVect_[i].SetIdxDestra( Ptr[i][3] );
        caselleVect_[i].SetIdxBasDes( Ptr[i][4] );
        caselleVect_[i].SetIdxBasCen( Ptr[i][5] );
        caselleVect_[i].SetIdxBasSin( Ptr[i][6] );
        caselleVect_[i].SetIdxSinist( Ptr[i][7] );
    }
}
//---------------------------------------------------------------------------

void TfrmMain::DisegnaSituazione( bool RefreshSfondo )
{
    // ========================================================
    Gdiplus::RectF R( VCLRectToGdiPlusRectF( TRect( 0, 0, larBoard_, altBoard_ ) ) );

    if( RefreshSfondo )
    {
        Gdiplus::ImageAttributes imgAttrs;

/*
String Res = RES_ZZ_LEGNO;
Gdiplus::Bitmap* Bmp  =  Gdiplus::Bitmap::FromResource( HInstance, Res.w_str() );

gdiBaseGhost_->DrawImage( Bmp, R, 0, 0, larBoard_, altBoard_, Gdiplus::UnitPixel, &imgAttrs );

Gdiplus::Color Col = TColorToGdiplusColor( TColor( 0x00FF3EFF ), 255 );
imgAttrs.SetColorKey( Col, Col, Gdiplus::ColorAdjustTypeDefault );
*/

        gdiBaseGhost_->DrawImage( bmpBoard_, R, 0, 0, larBoard_, altBoard_, Gdiplus::UnitPixel, &imgAttrs );

        DisegnaCaselle   ();
        DisegnaToccaA    ();
    }

    // ========================================================
    DrawImage( *gdiGhost_, *bmpBaseGhost_, R, 255, false, true );

    // ========================================================
    DisegnaLinea_D_D();  // D_D sta per Drag & Drop

    if( partitaFinita_ )
        DisegnaPercorsoWin();

//    DisegnaIdxCasella();

    // ========================================================
    Gdiplus::Graphics g( PaintBoxBoard->Canvas->Handle );
    DrawImage( g, *bmpGhost_, R, 255, false, true );


/*
    // serve a salvare la paintbox
    Graphics::TBitmap * Bmp = new Graphics::TBitmap( larBoard_, altBoard_ );
    Bmp->PixelFormat        = pf24bit;

    TRect R1( 0, 0, larBoard_, altBoard_ );
    Bmp->Canvas->CopyRect( R1, PaintBoxBoard->Canvas, R1 );

    Bmp->SaveToFile ( _T( "ZZ_Fondo_Ok.bmp" ) );

    delete Bmp;
*/

}
//---------------------------------------------------------------------------

void TfrmMain::DisegnaCaselle()
{
    Gdiplus::Color Blue = TColorToGdiplusColor( clBlue, 255 );

    // ===========================================================
    // traccio le linee
    Gdiplus::Pen pen( Blue, 6 ); // Penna, spessore
    pen.SetStartCap( Gdiplus::LineCapRound );
    pen.SetEndCap  ( Gdiplus::LineCapRound );

    for( UINT i = 0; i < caselleVect_.size(); i ++ )
    {
        TCasella* C = &caselleVect_[i];

        int Stato = C->GetStato();

        if( Stato == DIAGSIN )
        {
            int x1 = C->GetSpigoloX() + 4;
            int y1 = C->GetSpigoloY() + 4;
            int x2 = C->GetSpigoloX() + lato_ - 2;
            int y2 = C->GetSpigoloY() + lato_ - 2;

            gdiBaseGhost_->DrawLine( &pen, x1, y1, x2, y2 );
        }

        if( Stato == DIAGDES )
        {
            int x1 = C->GetSpigoloX() + lato_;
            int y1 = C->GetSpigoloY() + 4;
            int x2 = C->GetSpigoloX() + 4;
            int y2 = C->GetSpigoloY() + lato_;

            gdiBaseGhost_->DrawLine( &pen, x1, y1, x2, y2 );
        }
    }
}
//---------------------------------------------------------------------------

void TfrmMain::DisegnaIdxCasella()
{
    // stampo l'Idx della casella
    for( UINT i = 0; i < caselleVect_.size(); i ++ )
    {
        TCasella* C = &caselleVect_[i];

        int x = C->GetSpigoloX() + lato_ / 2;
        int y = C->GetSpigoloY() + lato_ / 2;
        ScriviTesto(  x, y, 28, clBlack, String( i ), gdiGhost_ );
    }
}
//---------------------------------------------------------------------------

void TfrmMain::DisegnaToccaA()
{
    ScriviTesto(  90, 30, 28, clLime, _T( "Player A" ), gdiBaseGhost_ );
    ScriviTesto( 800, 30, 28, clRed , _T( "Player B" ), gdiBaseGhost_ );

    if( !partitaFinita_ )
    {
        Gdiplus::Bitmap* Bmp1 = player_ == PLAYER ? bmpLedVerde_ : bmpLedSpent_;
        Gdiplus::Bitmap* Bmp2 = player_ == OPPOSI ? bmpLedRosso_ : bmpLedSpent_;
        int Lato =  50;

        Gdiplus::Color Fuchsia = TColorToGdiplusColor( (TColor)0x00FF3EFF, 255 );
        Gdiplus::ImageAttributes imgAttrs;
        imgAttrs.SetColorKey( Fuchsia, Fuchsia, Gdiplus::ColorAdjustTypeDefault );

        // ------------------------------------------------
        int xx1 =  80 - Lato / 2;
        int yy1 =  60;
        int xx2 = xx1 + Lato;
        int yy2 = yy1 + Lato;
        Gdiplus::RectF R1( VCLRectToGdiPlusRectF( TRect( xx1, yy1, xx2, yy2 ) ) );
        gdiBaseGhost_->DrawImage( Bmp1, R1, 0, 0, 50, 50, Gdiplus::UnitPixel, &imgAttrs );

        // ------------------------------------------------
        xx1 = 820 - Lato / 2;
        xx2 = xx1 + Lato;
        Gdiplus::RectF R2( VCLRectToGdiPlusRectF( TRect( xx1, yy1, xx2, yy2 ) ) );
        gdiBaseGhost_->DrawImage( Bmp2, R2, 0, 0, 50, 50, Gdiplus::UnitPixel, &imgAttrs );
    }
    else
    {
        if( playerWin_ == PLAYER )
        {
            ScriviTesto( 140, 100, 36, clLime, _T( "VITTORIA" ), gdiBaseGhost_ );
        }
        else
        {
            ScriviTesto( 760, 100, 36, clRed , _T( "VITTORIA" ), gdiBaseGhost_ );
        }
    }

}
//---------------------------------------------------------------------------

void TfrmMain::DisegnaLinea_D_D()
{
    if( !faseDrag_ ) return;

    int x1 = posizXdrag_ - latoPedina_ / 2;
    int y1 = posizYdrag_ - latoPedina_ / 2;
    int x2 = x1 + latoPedina_;
    int y2 = y1 + latoPedina_;

    Gdiplus::ImageAttributes imgAttrs;
    Gdiplus::RectF R( VCLRectToGdiPlusRectF( TRect( x1, y1, x2, y2 ) ) );
    gdiGhost_->DrawImage( bmpPedDrag_, R, 0, 0, lato_, lato_, Gdiplus::UnitPixel, &imgAttrs );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PaintBoxBoardMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    // ------------------------------------------------
    if( Button == mbRight )
    {
        partitaFinita_ = false;

        int Idx = ClickSuCasella( X, Y );
        if( Idx == NO_VAL )
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }

        int Pos = caselleVect_[Idx].GetStato();

        if     ( Pos == LIBERO  ) Pos = DIAGSIN;
        else if( Pos == DIAGSIN ) Pos = DIAGDES;
        else if( Pos == DIAGDES ) Pos = LIBERO ;

        caselleVect_[Idx].SetStato( Pos );

        CheckFinePartita();

        DisegnaSituazione( true );  // RefreshSfondo

        return;
    }

    // ------------------------------------------------
    // per il drag and drop
    mouseDown_  = false;

    faseDrag_   = false;
    posizXdrag_ = 0;
    posizYdrag_ = 0;

    // ------------------------------------------------
    if( partitaFinita_ ) return;

    Gdiplus::Color Verde = TColorToGdiplusColor( clLime, 255 );
    Gdiplus::Color Rosso = TColorToGdiplusColor( clRed , 255 );

    // ===========================================================
    Gdiplus::Pen Pv( Verde, 16 ); // Penna, spessore
    Pv.SetStartCap( Gdiplus::LineCapRound );
    Pv.SetEndCap  ( Gdiplus::LineCapRound );

    Gdiplus::Pen Pr( Rosso, 16 ); // Penna, spessore
    Pr.SetStartCap( Gdiplus::LineCapRound );
    Pr.SetEndCap  ( Gdiplus::LineCapRound );

    int IdxPed = ClickSuPedina( X, Y );
    if( IdxPed == PEDSIN )
    {
        bmpPedDrag_ = bmpPedDiagSin_;
        // disegno la diagonale del colore del player di turno
        Gdiplus::Graphics* G = new Gdiplus::Graphics( bmpPedDrag_ );

        if( player_ == PLAYER )
            G->DrawLine( &Pv, 0, 0, lato_, lato_ );
        else
            G->DrawLine( &Pr, 0, 0, lato_, lato_ );
    }

    if( IdxPed == PEDDES )
    {
        bmpPedDrag_ = bmpPedDiagDes_;
        // disegno la diagonale del colore del player di turno
        Gdiplus::Graphics* G = new Gdiplus::Graphics( bmpPedDrag_ );

        if( player_ == PLAYER )
            G->DrawLine( &Pv, lato_, 0, 0, lato_ );
        else
            G->DrawLine( &Pr, lato_, 0, 0, lato_ );
    }

    if( IdxPed == NO_VAL )   // non ho cliccato su una pedina
    {
        // ------------------------------------------------
        int Idx = ClickSuCasella( X, Y );
        if( Idx == NO_VAL )
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }

        if( CasellaProibita( Idx ) )
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }

        int Stato = caselleVect_[Idx].GetStato();
        if( Stato != LIBERO )
        {
            // inversione della diagonale
            if( Stato == DIAGSIN )
                caselleVect_[Idx].SetStato( DIAGDES );
            else
                caselleVect_[Idx].SetStato( DIAGSIN );

            CheckFinePartita();

            // cambio il giocatore
            player_ = player_ == PLAYER ? OPPOSI : PLAYER;

            DisegnaSituazione( true );  // RefreshSfondo

            return;
        }
        else
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }
    }

    // ------------------------------------------------
    faseDrag_   = true;
    inizDragX_  = X;
    inizDragY_  = Y;

    // ------------------------------------------------
    posizXdrag_ = X;
    posizYdrag_ = Y;

    // ------------------------------------------------
    mouseDown_  = true;
    DisegnaSituazione( true );  // RefreshSfondo

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PaintBoxBoardMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
    if( mouseDown_ )
    {
        posizXdrag_ = X;
        posizYdrag_ = Y;

        DisegnaSituazione( false );
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PaintBoxBoardMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    // ------------------------------------------------
    if( Button == mbRight )
    {
        mouseDown_  = false;
        return;
    }

    // ------------------------------------------------
    if( partitaFinita_ ) return;

    // ------------------------------------------------
    faseDrag_   = false;

    if( mouseDown_ )
    {
        // ------------------------------------------------
        int Idx = ClickSuCasella( X, Y );
        if( Idx == NO_VAL )
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }

        if( CasellaProibita( Idx ) )
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }

        if( caselleVect_[Idx].GetStato() != LIBERO )
        {
            PlaySound( RES_ERRORE, HInstance, SND_RESOURCE | SND_ASYNC );
            return;
        }

        if( bmpPedDrag_ == bmpPedDiagSin_ )
            caselleVect_[Idx].SetStato( DIAGSIN );
        else
            caselleVect_[Idx].SetStato( DIAGDES );

        CheckFinePartita();

        // cambio il giocatore
        player_ = player_ == PLAYER ? OPPOSI : PLAYER;

        DisegnaSituazione( true );
    }

    mouseDown_  = false;
}
//---------------------------------------------------------------------------

int TfrmMain::ClickSuPedina( int X, int Y )
{
    int x1 = spigoloPedSinX_;
    int y1 = spigoloPedSinY_;
    int x2 = spigoloPedSinX_ + lato_;
    int y2 = spigoloPedSinY_ + lato_;

    if( Y > y1 && Y < y2 && X > x1 && X < x2 )
    {
        return PEDSIN;
    }

    x1 = spigoloPedDesX_;
    y1 = spigoloPedDesY_;
    x2 = spigoloPedDesX_ + lato_;
    y2 = spigoloPedDesY_ + lato_;

    if( Y > y1 && Y < y2 && X > x1 && X < x2 )
    {
        return PEDDES;
    }

    return NO_VAL;
}
//---------------------------------------------------------------------------

int TfrmMain::ClickSuCasella( int X, int Y )
{
    for( UINT i = 0; i < caselleVect_.size(); i ++ )
    {
        TCasella* C = &caselleVect_[i];
        if( C->ClickSuCasella( X, Y ) != NO_VAL )
        {
            return i;   // � l'Idx della casella
        }
    }

    return NO_VAL;
}
//---------------------------------------------------------------------------

bool TfrmMain::CasellaProibita( int Idx )
{
    if( player_ == PLAYER )      // verde
    {
        if( Idx == 15 || Idx == 24 || Idx == 33 ||
            Idx == 23 || Idx == 32 || Idx == 41 )
            return true;
    }

    if( player_ == OPPOSI )      // rosso
    {
        if( Idx ==  0 || Idx ==  1 || Idx ==  2 ||
            Idx == 54 || Idx == 55 || Idx == 56 )
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------

void TfrmMain::CheckFinePartita()
{
    // PLAYER: dalla  1 alla 55
    if( player_ == PLAYER )
    {
        // controllo se il player ha vinto
        CheckWinner( 1, 55, PLAYER );
//        CheckWinner( 55, 1, PLAYER );

        // controllo se il player ha dato la vittoria all'OPPOSI
        if( !partitaFinita_ )
            CheckWinner( 24, 32, OPPOSI );
    }

    // OPPOSI: dalla 24 alla 32
    if( player_ == OPPOSI )
    {
        // controllo se l'OPPOSI ha vinto
        CheckWinner( 24, 32, OPPOSI );

        // controllo se l'OPPOSI ha dato la vittoria al PLAYER
        if( !partitaFinita_ )
            CheckWinner( 1, 55, PLAYER );
    }

}
//---------------------------------------------------------------------------

void TfrmMain::CheckWinner( int Idx, int IdxFine, int Quale )
{
    partitaFinita_ = false;
    percorsoWin_.clear();

    if( Idx >= 0 && Idx < caselleVect_.size() &&
        IdxFine >= 0 && IdxFine < caselleVect_.size() &&
        caselleVect_[Idx].GetStato() != LIBERO &&
        caselleVect_[IdxFine].GetStato() != LIBERO )
    {
        std::vector<int> precedente( caselleVect_.size(), -1 );
        std::vector<int> coda;
        precedente[Idx] = Idx;
        coda.push_back( Idx );

        for( UINT posizione = 0;
             posizione < coda.size() && precedente[IdxFine] == -1;
             posizione ++ )
        {
            int corrente = coda[posizione];
            TCasella* C = &caselleVect_[corrente];
            int Stato = C->GetStato();
            int vicini[8] =
            {
                C->GetIdxAltSin(), C->GetIdxAltCen(),
                C->GetIdxAltDes(), C->GetIdxDestra(),
                C->GetIdxBasDes(), C->GetIdxBasCen(),
                C->GetIdxBasSin(), C->GetIdxSinist()
            };

            for( int direzione = 0; direzione < 8; direzione ++ )
            {
                int vicino = vicini[direzione];
                if( vicino == NO_VAL || vicino < 0 ||
                    vicino >= caselleVect_.size() ||
                    precedente[vicino] != -1 )
                    continue;

                int StatoVicino = caselleVect_[vicino].GetStato();
                if( StatoVicino == LIBERO )
                    continue;

                bool diagonaleValida = false;
                if( Stato == DIAGSIN && ( direzione == 0 || direzione == 4 ) )
                    diagonaleValida = StatoVicino == Stato;
                else if( Stato == DIAGDES && ( direzione == 2 || direzione == 6 ) )
                    diagonaleValida = StatoVicino == Stato;
                else if( direzione == 1 || direzione == 3 ||
                         direzione == 5 || direzione == 7 )
                    diagonaleValida = StatoVicino != Stato;

                if( diagonaleValida )
                {
                    precedente[vicino] = corrente;
                    coda.push_back( vicino );
                }
            }
        }

        if( precedente[IdxFine] != -1 )
        {
            for( int corrente = IdxFine; corrente != Idx; corrente = precedente[corrente] )
                percorsoWin_.push_back( corrente );
            percorsoWin_.push_back( Idx );
            std::reverse( percorsoWin_.begin(), percorsoWin_.end() );

            partitaFinita_ = true;
            playerWin_ = Quale;
            EliminaDiagonaliFuoriPercorso();
            PlaySound( RES_VITTORIA3, HInstance, SND_RESOURCE | SND_ASYNC );
        }
    }

//    ListBox3->Visible = true;
    ListBox3->Clear();
    for( UINT i = 0; i < percorsoWin_.size(); i ++ )
        ListBox3->Items->Add( percorsoWin_[i] );

}
//---------------------------------------------------------------------------

void TfrmMain::EliminaDiagonaliFuoriPercorso()
{
    for( UINT i = 0; i < caselleVect_.size(); i ++ )
    {
        if( !InLista( i ) )
            caselleVect_[i].SetStato( LIBERO );
    }
}
//---------------------------------------------------------------------------

void TfrmMain::Ricorsiva( int Idx, int IdxFine, int Player )
{
    if( Idx == NO_VAL ) return;

    if( partitaFinita_ ) return;

    TCasella* C = &caselleVect_[Idx];

    if( C->GetTestata() ) return;   // gi� testata
    C->SetTestata( true );

    int Stato = C->GetStato();
    if( Stato != LIBERO )
    {
        percorsoWin_.push_back( Idx );

        // se � stata raggiunta la casella della vittoria
        if( Idx == IdxFine )
        {
            partitaFinita_ = true;

            if( Player == PLAYER )
            {
                playerWin_ = PLAYER;
            }

            if( Player == OPPOSI )
            {
                playerWin_ = OPPOSI;
            }

            PlaySound( RES_VITTORIA3, HInstance, SND_RESOURCE | SND_ASYNC );

            return;
        }



        // =====================================================
        int Iii;
        int Sta;

        // sequenza di controllo delle caselle
        // 1  2  3
        // 8     4
        // 7  6  5

        if( Stato == DIAGSIN )    // GetIdxAltSin
        {
            Iii = C->GetIdxAltSin();
            if( Iii != NO_VAL )
            {
                Sta = caselleVect_[Iii].GetStato();
                if( Sta == Stato && !caselleVect_[Iii].GetTestata() )
                    Ricorsiva( Iii, IdxFine, Player );
            }
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        Iii = C->GetIdxAltCen();     // GetIdxAltCen
        if( Iii != NO_VAL )
        {
            Sta = caselleVect_[Iii].GetStato();
            if( Sta != LIBERO && Sta != Stato && !caselleVect_[Iii].GetTestata() )
                Ricorsiva( Iii, IdxFine, Player );
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        if( Stato == DIAGDES )    // GetIdxAltDes
        {
            Iii = C->GetIdxAltDes();
            if( Iii != NO_VAL )
            {
                Sta = caselleVect_[Iii].GetStato();
                if( Sta == Stato && !caselleVect_[Iii].GetTestata() )
                    Ricorsiva( Iii, IdxFine, Player );
            }
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        Iii = C->GetIdxDestra();     // GetIdxDestra
        if( Iii != NO_VAL )
        {
            Sta = caselleVect_[Iii].GetStato();
            if( Sta != LIBERO && Sta != Stato && !caselleVect_[Iii].GetTestata() )
                Ricorsiva( Iii, IdxFine, Player );
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        if( Stato == DIAGSIN )    // GetIdxBasDes
        {
            Iii = C->GetIdxBasDes();
            if( Iii != NO_VAL )
            {
                Sta = caselleVect_[Iii].GetStato();
                if( Sta == Stato && !caselleVect_[Iii].GetTestata() )
                    Ricorsiva( Iii, IdxFine, Player );
            }
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        Iii = C->GetIdxBasCen();     // GetIdxBasCen
        if( Iii != NO_VAL )
        {
            Sta = caselleVect_[Iii].GetStato();
            if( Sta != LIBERO && Sta != Stato && !caselleVect_[Iii].GetTestata() )
                Ricorsiva( Iii, IdxFine, Player );
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        if( Stato == DIAGDES )    // GetIdxBasSin
        {
            Iii = C->GetIdxBasSin();
            if( Iii != NO_VAL )
            {
                Sta = caselleVect_[Iii].GetStato();
                if( Sta == Stato && !caselleVect_[Iii].GetTestata() )
                    Ricorsiva( Iii, IdxFine, Player );
            }
        }

        if( partitaFinita_ ) return;

        // ---------------------------------------------------
        Iii = C->GetIdxSinist();     // GetIdxSinist
        if( Iii != NO_VAL )
        {
            Sta = caselleVect_[Iii].GetStato();
            if( Sta != LIBERO && Sta != Stato && !caselleVect_[Iii].GetTestata() )
                Ricorsiva( Iii, IdxFine, Player );
        }
    }

}
//---------------------------------------------------------------------------

void TfrmMain::ScriviTesto( int x, int y, int h, TColor Col, String Testo, Gdiplus::Graphics * Gdi )
{
//    ShowMessage( Testo );

    // ----------------- font ------------------------------------
    float hText = h;
//    Gdiplus::Font TheFont( _T( "Arial" ), hText, Gdiplus::FontStyleBoldItalic );
//    Gdiplus::Font TheFont( _T( "Courier new" ), hText, Gdiplus::FontStyleBold );
    Gdiplus::Font TheFont( _D( "Arial" ), hText, Gdiplus::FontStyleBold );
    Gdiplus::StringFormat TheFormat;
    TheFormat.SetAlignment    ( Gdiplus::StringAlignmentCenter );
    TheFormat.SetLineAlignment( Gdiplus::StringAlignmentNear   );

    Gdi->SetTextRenderingHint( Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias );

    // ----------------- TextRect ------------------------------------
    Gdiplus::RectF BoundingBox;
    Gdi->MeasureString( Testo.c_str(), -1, &TheFont, Gdiplus::PointF( 0, 0 ), &TheFormat, &BoundingBox );
    float wText = BoundingBox.Width ;
    float aText = BoundingBox.Height;
//    Gdiplus::RectF TextRect( x - wText / 2, y - aText, wText, aText * 0.9F );
    Gdiplus::RectF TextRect( x - wText / 2, y - aText / 2, wText, aText * 0.9F );

    // ---------------- colore del testo -----------------------------
    Gdiplus::Color Colore = TColorToGdiplusColor( Col, 255 );
    SolidBrush TheBrush( Colore );

    // ---------------------------------------------------------------
    // scrivo il testo
    Gdi->DrawString( Testo.c_str(), -1, &TheFont, TextRect, &TheFormat, &TheBrush );

}
//---------------------------------------------------------------------------

void TfrmMain::DisegnaPercorsoWin()
{
//ListBox1->Visible = true;
ListBox1->Clear();

    Gdiplus::Color Verde = TColorToGdiplusColor( clLime, 255 );
    Gdiplus::Color Rosso = TColorToGdiplusColor( clRed , 255 );

    // ===========================================================
    // traccio le linee della vittoria
    Gdiplus::Pen PenV( Verde, 12 ); // Penna, spessore
    PenV.SetStartCap( Gdiplus::LineCapRound );
    PenV.SetEndCap  ( Gdiplus::LineCapRound );

    Gdiplus::Pen PenR( Rosso, 12 ); // Penna, spessore
    PenR.SetStartCap( Gdiplus::LineCapRound );
    PenR.SetEndCap  ( Gdiplus::LineCapRound );

    for( UINT i = 0; i < percorsoWin_.size(); i ++ )
    {
        int Idx = percorsoWin_[i];


ListBox1->Items->Add( Idx );


        TCasella* C = &caselleVect_[Idx];

        int Stato = C->GetStato();

        // Stato == DIAGSIN
        int x1 = C->GetSpigoloX() + 4;
        int y1 = C->GetSpigoloY() + 4;
        int x2 = C->GetSpigoloX() + lato_ - 2;
        int y2 = C->GetSpigoloY() + lato_ - 2;

        if( Stato == DIAGDES )
        {
            x1 = C->GetSpigoloX() + lato_;
            y1 = C->GetSpigoloY() + 4;
            x2 = C->GetSpigoloX() + 4;
            y2 = C->GetSpigoloY() + lato_;
        }

        if( playerWin_ == PLAYER )
            gdiGhost_->DrawLine( &PenV, x1, y1, x2, y2 );
        else
            gdiGhost_->DrawLine( &PenR, x1, y1, x2, y2 );
    }

}
//---------------------------------------------------------------------------

void TfrmMain::TogliRamiSecchi()
{
//ListBox2->Visible = true;
ListBox2->Clear();

for( UINT i = 0; i < percorsoWin_.size(); i ++ )
    ListBox2->Items->Add( percorsoWin_[i] );


    do
    {
        bool Eliminato = false;
        for( UINT i = 0; i < percorsoWin_.size(); i ++ )
        {
            int  Idx = percorsoWin_[i];
            bool RamoSecco = CheckRamoSecco( Idx );

            if( RamoSecco )
            {
                // devo togliere l'Idx dal vettore
                TogliIdx( Idx );

                Eliminato = true;
                break;
            }
        }

        if( !Eliminato ) break;

    } while( true );
}
//---------------------------------------------------------------------------

bool TfrmMain::CheckRamoSecco( int Idx )
{
    // controllo le caselle di partenza e di arrivo
    if( playerWin_ == PLAYER )
    {
        TogliIdx( 24 );
        TogliIdx( 32 );

        if( Idx ==  1 || Idx == 55 ) return false;   // non � un ramo secco
    }

    if( playerWin_ == OPPOSI )
    {
        TogliIdx(  1 );
        TogliIdx( 55 );

        if( Idx == 24 || Idx == 32 ) return false;   // non � un ramo secco
    }





//return false;





    // -------------------------------------------------
    TCasella* C = &caselleVect_[Idx];

    int Stato = C->GetStato();

    if( Stato == DIAGSIN )
    {
        int Id1 = C->GetIdxAltSin();
        int Id2 = C->GetIdxAltCen();
        int Id3 = C->GetIdxSinist();

        bool Sec1 = Check3DiaSinAlt( Id1, Id2, Id3 );

        int Id4 = C->GetIdxDestra();
        int Id5 = C->GetIdxBasDes();
        int Id6 = C->GetIdxBasCen();

        bool Sec2 = Check3DiaSinBas( Id4, Id5, Id6 );

        if( Sec1 || Sec2 ) return true;    // ramo secco
    }

    // =================================================
    // =================================================
    if( Stato == DIAGDES )
    {
        int Id1 = C->GetIdxAltCen();
        int Id2 = C->GetIdxAltDes();
        int Id3 = C->GetIdxDestra();

        bool Sec1 = Check3DiaDesAlt( Id1, Id2, Id3 );

        int Id4 = C->GetIdxAltSin();
        int Id5 = C->GetIdxBasSin();
        int Id6 = C->GetIdxBasCen();

        bool Sec2 = Check3DiaDesBas( Id4, Id5, Id6 );

        if( Sec1 || Sec2 ) return true;    // ramo secco
    }

    return false;  // NON � un ramo secco
}
//---------------------------------------------------------------------------

bool TfrmMain::Check3DiaSinAlt( int Id1, int Id2, int Id3 )
{
    int Sta1 = LIBERO;
    int Sta2 = LIBERO;
    int Sta3 = LIBERO;
    // controllo le 3 caselle papabili dello spigolo sinistro in alto
    if( Id1 != NO_VAL )
    {
        Sta1 = caselleVect_[Id1].GetStato();
        if( Sta1 == DIAGSIN && !InLista( Id1 ) )
            return false;
    }

    if( Id2 != NO_VAL )
    {
        Sta2 = caselleVect_[Id2].GetStato();
        if( Sta2 == DIAGDES && !InLista( Id2 ) )
            return false;
    }

    if( Id3 != NO_VAL )
    {
        Sta3 = caselleVect_[Id3].GetStato();
        if( Sta3 == DIAGDES && !InLista( Id3 ) )
            return false;
    }

    if( Sta1 == LIBERO && Sta2 == LIBERO && Sta3 == LIBERO )
        return true;   // E' un ramo secco

    return false;  // NON � un ramo secco
}
//---------------------------------------------------------------------------

bool TfrmMain::Check3DiaSinBas( int Id1, int Id2, int Id3 )
{
    int Sta1 = LIBERO;
    int Sta2 = LIBERO;
    int Sta3 = LIBERO;
    // controllo le 3 caselle papabili dello spigolo sinistro in alto
    if( Id1 != NO_VAL )
    {
        Sta1 = caselleVect_[Id1].GetStato();
        if( Sta1 == DIAGDES && !InLista( Id1 ) )
            return false;
    }

    if( Id2 != NO_VAL )
    {
        Sta2 = caselleVect_[Id2].GetStato();
        if( Sta2 == DIAGSIN && !InLista( Id2 ) )
            return false;
    }

    if( Id3 != NO_VAL )
    {
        Sta3 = caselleVect_[Id3].GetStato();
        if( Sta3 == DIAGDES && !InLista( Id3 ) )
            return false;
    }

    if( Sta1 == LIBERO && Sta2 == LIBERO && Sta3 == LIBERO )
        return true;   // E' un ramo secco

    return false;  // NON � un ramo secco
}
//---------------------------------------------------------------------------

bool TfrmMain::Check3DiaDesAlt( int Id1, int Id2, int Id3 )
{
    int Sta1 = LIBERO;
    int Sta2 = LIBERO;
    int Sta3 = LIBERO;
    // controllo le 3 caselle papabili dello spigolo sinistro in alto
    if( Id1 != NO_VAL )
    {
        Sta1 = caselleVect_[Id1].GetStato();
        if( Sta1 == DIAGSIN && !InLista( Id1 ) )
            return false;
    }

    if( Id2 != NO_VAL )
    {
        Sta2 = caselleVect_[Id2].GetStato();
        if( Sta2 == DIAGDES && !InLista( Id2 ) )
            return false;
    }

    if( Id3 != NO_VAL )
    {
        Sta3 = caselleVect_[Id3].GetStato();
        if( Sta3 == DIAGSIN && !InLista( Id3 ) )
            return false;
    }

    if( Sta1 == LIBERO && Sta2 == LIBERO && Sta3 == LIBERO )
        return true;   // E' un ramo secco

    return false;  // NON � un ramo secco
}
//---------------------------------------------------------------------------

bool TfrmMain::Check3DiaDesBas( int Id1, int Id2, int Id3 )
{
    int Sta1 = LIBERO;
    int Sta2 = LIBERO;
    int Sta3 = LIBERO;
    // controllo le 3 caselle papabili dello spigolo sinistro in alto
    if( Id1 != NO_VAL )
    {
        Sta1 = caselleVect_[Id1].GetStato();
        if( Sta1 == DIAGSIN && !InLista( Id1 ) )
            return false;
    }

    if( Id2 != NO_VAL )
    {
        Sta2 = caselleVect_[Id2].GetStato();
        if( Sta2 == DIAGDES && !InLista( Id2 ) )
            return false;
    }

    if( Id3 != NO_VAL )
    {
        Sta3 = caselleVect_[Id3].GetStato();
        if( Sta3 == DIAGSIN && !InLista( Id3 ) )
            return false;
    }

    if( Sta1 == LIBERO && Sta2 == LIBERO && Sta3 == LIBERO )
        return true;   // E' un ramo secco

    return false;  // NON � un ramo secco
}
//---------------------------------------------------------------------------

bool TfrmMain::InLista( int Idx )
{
    for( UINT i = 0; i < percorsoWin_.size(); i ++ )
    {
        if( percorsoWin_[i] == Idx )
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------

void TfrmMain::TogliIdx( int Idx )
{
    for( UINT i = 0; i < percorsoWin_.size(); i ++ )
    {
        if( percorsoWin_[i] == Idx )
        {
            // tolgo l'Idx dal vettore
            percorsoWin_.erase( percorsoWin_.begin() + i );
            break;
        }
    }
}
//---------------------------------------------------------------------------
