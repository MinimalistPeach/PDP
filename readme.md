# Párhuzamos eszközök programozása repository

## Féléves beadandó feladat: *Részecske mozgás szimuláció*

A féléves beadandó feladatom témája a részecskék véletlenszerű mozgása köré épül. 
A probléma méretét a részecskék száma határozza meg, ez a program belépési pontjában van definiálva:

```#define NUM_PARTICLES 1000000```

Ezenkívül meg van határozva a DT változó, ami az idő differenciát kívánja szimulálni. Ez a konstans a mozgáshoz szükséges, mivel egy fizikai szimulációról van szó. Ez egy szorzóként szolgál a részecske sebességének kiszámításában.

``` const float DT = 1.2f; ```

A program main függvényében létrejön a részecske tömb, ami a részecskék számának megfelelő helyet foglal le a memóriában.

``` Particle *particles = (Particle *)malloc(NUM_PARTICLES * sizeof(Particle)); ```

A véletlenszerű számok tömbjének a deklarálása is itt történik meg, aminek a száma megegyezik a részecskék számának kétszeresével. Ezek a véletlenszerű számok kerülnek felhasználásra a részecske új sebességének kiszámításában. Mivel a részecskék számának kétszerese a tömb mérete, ezáltal a részecske tömb indexének kétszerese adja meg a részecskéhez tartozó véletlenszerű számot.

``` float *randoms = (float *)malloc(2 * NUM_PARTICLES * sizeof(float)); ```

A belépési ponton két függvény hívása történt még, az egyik a CPU-n hajtja végre a részecske mozgatást, a másik ezzel ellentétben a GPU-n. A CPU hívó függvény egyik paramétere a szálak száma, ennek a számnak a változtatásával növelhető vagy csökkenthető a CPU futási idő erre a problémára.

## CPU végrehajtás
Legelőször szétosztja a szálak között a probléma méretet, ezáltal a részecskék bizonyos részének pozícióját és sebességét számítja 1-1 szál.

## GPU végrehajtás
GPU végrehajtás esetén először inicializálásra kerül a részecske és a véletlenszerű szám buffer, amelyet a kernel fog felhasználni. Ezt követően a kernel-be arugmentumokként vannak megadva a különböző számításhoz szükséges számok. Ezen számok nagy része véletlenszerű. Ezt azért szükséges arugmentumként átadni, mivel az OpenCL-ben nem vagy csak nagyon nehezen lehet véletlenszerű számot generálni.

