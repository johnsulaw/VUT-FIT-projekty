VUT FIT IJA 2021 - Simulace skladu

Autoři: Jan Šulavík (xsulav01)
	Michal Polonec (xpolon02)

Použití:
	1. Ve složce 'lib' spustit ./get-libs.sh - pro stažení potřebných knihoven
	2. ant compile
	3. ant run

Programová dokumentace se vygeneruje použizím 'ant doc' v kořenovém adresáři.

Specifikace požadavků

    Základní požadavky
        aplikace zobrazí mapový podklad skladu, na který poté přenáší informace o pohybu vozíků
            základní mapový podklad je tvořen cestami, regály a výdejním/nákladním místem
            tento základní koncept můžete jakkoliv rozšířit – např. o parkování volných vozíků
            mapový podklad se načte po spuštění ze souboru (formát je na vašem uvážení)
            mapový podklad je možné přibližovat a oddalovat (zoom)
        systém regálů
            každý regál musí obsahovat alespoň jeden typ zboží (vč. informace o počtu kusů; může být i prázdný)
            aplikace umožní inicializaci skladu načtením dat ze souboru (formát je na vás)
            aplikace bude podporovat minimálně 50 druhů zboží
            po najetí/kliknutí na pozici regálu se zobrazí jeho aktuální obsah
        systém požadavků
            požadavek je soupis zboží a počet kusů, které se mají převézt na výdejní místo
            aplikace umožňuje načítání požadavků ze souboru i ruční zadávání
        systém vozíků
            vozíky si sami určují (pokud možno efektivní) cestu ke splnění požadavku
            každá cesta je definována seznamem regálů ke nakládá zboží (výdejní místo – regál 1 - … - regál x - výdejní místo)
            je třeba zohlednit kapacitu vozíku
                postačí jedno kritérium (např. maximální počet zboží na vozík 5ks nebo maximální váha zboží 100kg)
                není třeba kritéria kombinovat (ale můžete, v reálném prostředí je nutné uvažovat váhu, rozměry atd.)
                můžete zvážit různé typy vozíky s různými hodnotami parametrů
            naložení zboží na vozík trvá stejnou dobu
    Pohyb vozíků
        systém obsahuje vlastní hodiny, které lze nastavit na výchozí hodnotu a různou rychlost
        po načtení mapy a obsahu skladu začne systém zobrazovat zpracování jednotlivých požadavků (způsob zobrazení je na vaší invenci, postačí značka, kolečko, ...)
        symbol vozíku se postupně posunuje podle aktuálního času a požadavků (aktualizace zobrazení může být např. každých N sekund); pohyb spoje na trase je tedy simulován
        po najetí/kliknutí na symbol vozíku se zvýrazní trasa v mapě a zobrazí jeho aktuální náklad
    Interaktivní zásahy
        Je možné definovat možnost uzavření uličky
            vozík si automaticky zvolí objízdnou trasu (a aktualizuje ji i ve vizualizaci trasy)
            objízdná trasa může vynechat některou ze zastávek, pokud není možné požadavek obsloužit jinak
