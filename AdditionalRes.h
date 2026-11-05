#ifndef ADDITIONALRES_H
#define ADDITIONALRES_H

// ===============================================
#define RES_RULES              _T( "RULES" )

// ===============================================
#define RES_ZZ_LEGNO           _T( "ZZ_LEGNO"   )
#define RES_ZZ_FONDO           _T( "ZZ_FONDO"   )

#define RES_ZZ_PEDDIAGSIN      _T( "ZZ_PEDDIAGSIN"   )
#define RES_ZZ_PEDDIAGDES      _T( "ZZ_PEDDIAGDES"   )

#define RES_ZZ_LEDVERDE        _T( "ZZ_LEDVERDE" )
#define RES_ZZ_LEDROSSO        _T( "ZZ_LEDROSSO" )
#define RES_ZZ_LEDSPENT        _T( "ZZ_LEDSPENT" )


#endif

/*
Per mettere i jpg (o altra roba binaria):

Il file .rc deve essere fatto così:

#include "additionalres.h"

SPLASH       RCDATA  "Resources\Splash.jpg"
DEFPALETTE   RCDATA  "Resources\defpalette.xml"

e un file .h (additionalres.h) siffatto:

#ifndef ADDITIONALRES_H
#define ADDITIONALRES_H

#define  SPLASH_RES          "SPLASH"
#define  DEF_PALETTE         "DEFPALETTE"

#endif

Poi si accede ai blocchi così:

#include "AdditionalRes.h"

        auto_ptr<TResourceStream> const ResourceStream(
            new TResourceStream( (unsigned)HInstance, _T( SPLASH_RES ), RT_RCDATA )
        );
        auto_ptr<TJPEGImage> const JPG( new TJPEGImage );
        JPG->LoadFromStream( ResourceStream.get() );
        Image->Picture->Bitmap->Assign( JPG.get() );

Oppure così:

        scoped_ptr<TResourceStream> const ResourceStream(
            new TResourceStream( (unsigned)HInstance, _T( DEF_PALETTE ), RT_RCDATA )
        );
        palette_.LoadFromStream( *ResourceStream );

*/
