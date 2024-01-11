# 1\_sudoku\_report

## Terminologija

- box
  - podmatrica koja u sebi mora da sadrzi sve brojeve
  - npr 3x3 ima od 1 do 9, 4x4 od 1 do 16...

- singleton 
  - polje koje ima samo jedan moguc broj koji moze da se stavi u nejga

- peer
  - svako polje u odnosu na posmatrano koje se poklapa po: horizontali, vertikali, box-u
  
- Postoje dva dela algoritma
  - Constraint propagation (CP)
  - Search method (SM)

- constraint propagation
  - moraju se zadovoljiti dva zahteva:
    1. Svaki broj peerova koji su singleton izbacimo iz mogucnosti za trenutno polje i ako ostane samo jedan broj to je to
    2. Ako svi peerovi imaju izbaceno iz seta neki broj, to znaci da taj broj odgovara trenutno posmatranom polju

- search method
  - kada vise ne moze da se dodje do resenja samo koriscenjem pravila sudokua, onda se mora pokusavati nasumicnim pokusajima
  - tako se za svako potencijalno resenje nekog polja pravi podgrana resenja i jedna nit moze da uhvati da obradjuje jednu granu

- U ovom radu tok resavanja je bio: CP -> SC -> CP -> SC -> ...

- CP ne moze da se paralelizuje zbog toga sto svaki novi upis u bilo koji deo matrice moze da utice na potencijalna resenja ostalih polja
- SC moze da se ozbiljno parallizuje, gde bi svaka nit uzela po jedno moguce resenje uradila opet CP i onda proverila moguca resenjea itd.  

- Korisceno je nekoliko pristupa paralelelizaciji, ali se oni mogu generalizovati na dva
  - Message passing gde kada jedna nit ostane bez posla ona, da bi rasteretila ostale niti, trazi da joj delegiraju posao (**moguce: openMPI**)
    - ono sto ovde moze da se bolje odradi od njih je sto svaka nit moze da delegira posao samo susednoj niti (susedan ID)
  - Locking, gde se sva moguca resenja skladiste u deljenoj memoriji i onda niti zakljucavaju tu memoriju, uzmu granu za sebe i krenu da resvaaju (**Moguce openMP**)
    - dva pristupa:
      - zakljucava se celo stablo (performanse kao serijska implementacija)
      - zakljucava se (valjda, procitaj bolje kasnije) samo jedna grana i to samo u trenutku kada se dodaje ili skida sa nje 

- Referencirani radovi koje bi bilo dobro procitati:
  - P\. Norvig, “Solving every sudoku puzzle.”: http://www.norvig.com/sudoku.html
  - DryIcons, “A simple algorithm for generating sudoku puzzles.”: http://dryicons.com/blog/2009/08/14/a-simple-algorithm-for-generating-sudoku-puzzles/


# 2\_multi\_threaded\_sudoku

- ovaj citav rad se bazira na prethodnom, prethodni je cak nudio vise resenja paralelizacije, ovaj samo jedan
- jedino novo sto sam saznao je kako cuvati informaciju u potencijalnim vrednostima cell-a
  - brojevi od 1 do N cuvaju se u binarnoj reprezentaciji , npr ako imam 9x9 sudoku imacemo 9bitni broj u kome ce biit smestene sve mogucnosti binarno
- U ovom radu je dat neki pseudo kod za message passing
- Performanse u ovom radu bile su njabolje za 3 niti, sa povecavanjem, doslo je do prevelikog overheada sa komunikacijom niti


- ono sto je mana oba rada je komunikacija niti samo sa susednim nitima

# 3\_mpi\_sudoku\_solving

- ovaj rad uvodi nove pristupe ressavanju preko algoritama X (resava exact cover problem) koji je implementairan pomocu Dancing Links strukture koju je razvio Knuth
  - dancing links je karakteristican po tome sto prilikom brisanja cvora iz dl liste on ipak pamti njegov polozaj, pa ga je lakse vratiti nazad nakon brisanja ako se za to javi potreba
    - ovo je korisno kod back tracking algoritama, kod kojih se javalja cesta potreba za undoovanjem stvari
- Ono sto je meni ovde bilo od znacaja je novi MPI pristup : **One sided communication**
  - zasniva se na tome da nema send receive mehanizma, nego jedan proces stavlja neku info u svoj memorijski prozor za deljenje memorije, kome ostali procesi mogu da pristupe
    - na ovaj nacin imamo jednog pasivnog i jednog aktivnog ucesnika u komunkaciji
  - ovaj pristup komunikaciji davao je bolje performanse, a obrzalozenej je bilo sto send receive mora da koristi jednog jezgro za kooordinaciju procesa, sto uvek smanjuje performanse u odnosu na pristup gde se koristi i to jedno jezgro
  - u ovom radu takodje korsiten je mnogo jaci racunar u odnosu na prethodne radove (Epic sa 64 jezgra  128 threadova, 2-3.5GHz, prehodni su imali 4 jezgra sa 2-3GHZ)

- iz ovoga sam zakljucio da bih za MPI implementaciju mogao korsititi one sided komunikaciju i nekako uspeti da omogucim da svaki proces prica sa svakim i time poboljsam algoritme iz sourceva 1 i 2

# 4\_sudoku\_algorithms\_bsc

- u ovom radu lepo su definisani osnovni pojmovi sudokua u poglavlju jedan (Definitions)
  - fali samo singleton
- ovaj radi manje vise koristi takodje constraint propagation i search, ali ih drugacije naziva
  - jedina razlika je sto je ovaj constraint propagation algoritam ima malo vise pravila
- jos jedna metoda je navedena a to je Boltzmanova masina koja se koristi neuronskim mrezama

- ovaj rad nije imao za cilja paralelizaciju, tako da je samo pomenuo mogucnosti, nije radjena implementacija


- iz ovog rada mogu se izvuci lepe definicije i moguce poboljsanje contraint propagtaion algoritma za prethodne radove

# Moj rad

## Generisanje sudoku puzle i resenja radi provere

- pronasao sam gotovo resenje na sajtu: https://sudokutodo.com/generator
  - podesiti da se na prazna mesta napise x
  - prezueti zadatak i resenje i odna samo misem uraditi copy paste u vim
  - u vimu na pomocu makroa pretvoriti copirani tekst u csv fajl
  - na kraju ovoga smo obezbedili ulaz i zeljeni izlaz algoritma

## Constraint propagation
- napisao sam svoju implementaciju koja je malo optimizovala koriscenje memorije tako sto se umesto niza cifara(ili karaktera) koristion jedan unsigned int koji je informacije o jednom broju cuvao unutar jednog bita
  - ovo je neke delove implementacije zakomplikovalo, dok je je priroda binarnih operacija veoma lepo nalegla na ideju izbacivanja potencijalnih vrednosti za jednu celiju

## Search

- search se zasniva na dfs algoritmu koji ce biti tema dalje paralelizacije

## Testiranje

- dve mogucnosti u zavisnosti od preciznosti njihovih merenja
  - high precision clock unutar samog programa, pa da se rezultat ispisuje na izlazi
  - alat poput linux "time"

- u oba slucaja, pisace se shell skripta koja ce pokretati test scenario
