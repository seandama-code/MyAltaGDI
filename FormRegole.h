//---------------------------------------------------------------------------

#ifndef FormRegoleH
#define FormRegoleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmRegole : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TPanel *Panel1;
    TBitBtn *BitBtnConferma;
    TPanel *Panel2;
    TPanel *Panel3;
    TImage *Image1;
    TImage *Image2;
    TImage *Image3;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall BitBtnConfermaClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRegole(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmRegole *frmRegole;
//---------------------------------------------------------------------------
#endif
