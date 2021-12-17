# klient_windows_app

Tento klient je naprogramovaný v c++ pre windows aplikácie. Teda má svoje windows okno. V ktorom prebieha aplikácia.  No momentále sa nachádzajú v ľavej časti okna dva stĺpce.
Prvý stĺpec obsahuje voľbu pre určenie svojeho "unikátneho ID". Pre jednoduchosť si každy klient vyberie sám toto číslo pod ktorým bude komunikovať. Inak by sa to dalo vyriešiť cez nejaký
prihlasovací systém na servery.  Následne druhý stĺpec obsahuje voľbu ktorému používateľovi ide písať.   Ďalej následuje štvorec ktorý predstavuje vstup z klávesnice.
Tu sa napíše správa o vopred určenej maximálnej veľkosti. Tento vstupný štvorec je ošetrený nech sa nemôže písať mimo neho. Následne sú v pravej časti
dve červené tlačídla. Jedno je pre odoslanie správy s názvom "Pošli správu". stane sa vlastne to, že správa zo štvorca sa uloží do objektu message.
Trimne sa od konca všetký prázdne znaky zmaže. takže sa odošle serveru len to čo je treba. a na začiatok odosielaných dát sa v objekte message ešte v konštruktore
appendne na zaČiatok formáť 1|12|    teda v konečnom dôsledku sa odošle serveru toto 1|12|message+/0  Čiže informácia od koho, komu, aká správa. A ukončená nulou pre jednoduhšiu
prácu na servery.

Najťažší je pravý stĺpec. TEn bude slúžiť na zobrazovanie chatu. Posledných poslaných správ s daným uživateľom. Bude sa plniť tak, že vždy keď odošlem správu uživateľovi buď ja,
alebo uživateľ ktorému to posielam, tak server keď uloží tieto správy do súboru, tak ich nazad odošle obom týmto klientom. a klienti si ich už následovne vykreslia v tomto stĺpci,
aby videli čo si medzi sebou naposledy písali.  Je to akoby real time databaza. Ibaže riešená cez sokety.

Nápad je taký, že klient bude neustále počúvať na jednom vlákne či mu náhodou neprišla správa zo servera. ktorá bude obsahovať aktualizáciu chatu. Na hlavnom vlákne,
bude  bežať klasická windows apka windows okno.   Server by mal mať Jedno vlákno s ktorým posiela dáta. A x vlakien pre každého klienta na ktorých bude počúvať čo za data chodí od nich,
čo oni odoslali.   v podstate do tohto súboru správ bude zapisovať viacero vlákien. Čiže tento súbor sa bude musieť ošetriť ako atomická premenná nech nedôjde ku kolízí vlákien.
