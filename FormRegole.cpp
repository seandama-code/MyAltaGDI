//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <tchar.h>
#include "AdditionalRes.h"

#include "FormRegole.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmRegole *frmRegole;
//---------------------------------------------------------------------------
__fastcall TfrmRegole::TfrmRegole(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmRegole::FormShow(TObject *Sender)
{
    Height = 900;
    Color = clWhite;

    Image1->Top +=  40;
    Image2->Top +=  80;
    Image3->Top += 120;

    String Resource = RES_RULES;
    TResourceStream* theResStream = new TResourceStream( (NativeUInt)HInstance, Resource.c_str(), RT_RCDATA );

    Memo1->Lines->Clear();
    Memo1->Lines->LoadFromStream( theResStream );

    delete theResStream;

/*


    HRSRC hResource = FindResource( HInstance, RES_RULES, RT_RCDATA );

    if (hResource)
    {
        HGLOBAL hLoadedResource = LoadResource(HInstance, hResource);

        if (hLoadedResource)
        {
            LPVOID pLockedResource = LockResource( hLoadedResource );

            if (pLockedResource)
            {
                DWORD dwResourceSize = SizeofResource(HInstance, hResource);

                if (0 != dwResourceSize)
                {
                    // Use pLockedResource and dwResourceSize however you want
                    Memo1->Lines->Add( (char*)pLockedResource );
                }
            }
        }
    }
*/

}
//---------------------------------------------------------------------------

void __fastcall TfrmRegole::BitBtnConfermaClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
