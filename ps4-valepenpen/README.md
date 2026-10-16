# ValePenPen: Pipì alla riscossa — PS4 demo 01

Prototipo nativo PS4 **per un primo test su console modificata**.

Contenuti:
- Pavimento isometrico in rovere miele da `assets/floors/01_parquet/rovere_miele_iso_01_512x256.png`.
- Due muri originali da `assets/walls/01_base/`.
- 64 frame V2 reali da `assets/characters/valentina/walk/8_directions_v2_review/`.
- Comandi DualShock 4: stick sinistro / frecce direzionali = movimento; triangolo = visualizza indicatore di test.
- Collisione di prova con i limiti rettangolari della stanza (non ancora pareti fisiche).
- Render 2D software 1280×720; no luci in tempo reale, no audio, no missioni.

**Attenzione:** i 64 frame V2 contengono ancora passi poco diversi tra loro. È un test di integrazione e animazione, non l'animazione finale. L'immagine generata nell'ultima chat non è stata usata perché cambia il look approvato.

## GitHub Actions

Repository di test: `lamelarossa2-debug/foxhollow`.
Ramo: `valepenpen-ps4-prototype`.
Workflow: `.github/workflows/valepenpen-ps4-prototype.yml`.
Artifact atteso: `VALEPENPEN-PS4-DEMO-01` (contiene `.pkg`).

Il progetto originale e le vecchie demo non vengono cancellati o riscritti.

## Sorgenti

- `ps4-valepenpen/main.cpp`: rendering e input PS4.
- `ps4-valepenpen/packed_art.h`: immagini ricavate dai PNG originali, compresse con RLE RGB 40 colori, decodificate in RAM.
- `build_art.py`: riproduce il pacchetto asset dai file originali sul nostro sistema di lavoro.

La versione compilata della demo non convalida il gameplay finale; la console deve essere testata manualmente.