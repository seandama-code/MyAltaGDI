//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Buttons.hpp>

#include <vector>
#include <boost/scoped_ptr.hpp>

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *LabelTitolo;
    TBitBtn *BitBtnNuovaPartita;
    TButton *ButtonRegole;
    TPaintBox *PaintBoxBoard;
    TListBox *ListBox1;
    TRadioGroup *RadioGroupAvversario;
    TPanel *PanelBoard;
    TListBox *ListBox2;
    TListBox *ListBox3;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall PaintBoxBoardPaint(TObject *Sender);
    void __fastcall PaintBoxBoardMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PaintBoxBoardMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
    void __fastcall PaintBoxBoardMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall BitBtnNuovaPartitaClick(TObject *Sender);
    void __fastcall ButtonRegoleClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations

    // ---------------------------------------------
    // la parte grafica
    int    larBoard_;
    int    altBoard_;
    Gdiplus::Bitmap  * bmpBoard_   ;

    Gdiplus::Bitmap  * bmpPedDiagSin_;
    Gdiplus::Bitmap  * bmpPedDiagDes_;

    Gdiplus::Bitmap  * bmpLedVerde_;
    Gdiplus::Bitmap  * bmpLedRosso_;
    Gdiplus::Bitmap  * bmpLedSpent_;

    // ---------------------------------------------
    Gdiplus::Bitmap  * bmpBaseGhost_;
    Gdiplus::Graphics* gdiBaseGhost_;

    Gdiplus::Bitmap  * bmpGhost_;
    Gdiplus::Graphics* gdiGhost_;

    // ---------------------------------------------
    int    lato_;
    CasellaVect caselleVect_;
    void   CreaCaselle();

    // ---------------------------------------------
    void   DisegnaSituazione ( bool RefreshSfondo );

    void   DisegnaCaselle    ();
    void   DisegnaToccaA     ();

    void   DisegnaLinea_D_D  ();     // D_D sta per Drag & Drop

    void   DisegnaIdxCasella ();

    // -----------------------------------------------------
    // per il drag and drop
    bool  mouseDown_   ;

    bool  faseDrag_    ;

    int   inizDragX_   ;     // X di inizio del drag
    int   inizDragY_   ;     // Y di inizio del drag
    int   posizXdrag_  ;     // X del drag
    int   posizYdrag_  ;     // Y del drag

    // ---------------------------------------------
    // coordinate degli spigoli delle pedine
    int spigoloPedSinX_;
    int spigoloPedSinY_;
    int spigoloPedDesX_;
    int spigoloPedDesY_;

    // click su una pedina o una casella
    int    latoPedina_;
    Gdiplus::Bitmap  * bmpPedDrag_;
    int   ClickSuPedina  ( int X, int Y );
    int   ClickSuCasella ( int X, int Y );
    bool  CasellaProibita( int Idx );

    int   player_       ;
    int   playerWin_    ;
    bool  partitaFinita_;
    void  CheckFinePartita();
    void  CheckWinner     ( int Idx, int IdxFine, int Quale  );
    void  EliminaDiagonaliFuoriPercorso();
    void  Ricorsiva       ( int Idx, int IdxFine, int Player );
    std::vector<int> percorsoWin_;
    void  DisegnaPercorsoWin();

    void  ScriviTesto( int x, int y, int h, TColor Col, String Testo, Gdiplus::Graphics * Gdi );

    void  TogliRamiSecchi();
    bool  CheckRamoSecco ( int Idx );
    bool  Check3DiaSinAlt( int Id1, int Id2, int Id3 );
    bool  Check3DiaSinBas( int Id1, int Id2, int Id3 );
    bool  Check3DiaDesAlt( int Id1, int Id2, int Id3 );
    bool  Check3DiaDesBas( int Id1, int Id2, int Id3 );
    bool  InLista        ( int Idx );
    void  TogliIdx       ( int Idx );

public:		// User declarations
    __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
