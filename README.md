# Kolik

Nástroj pro testování programů.

## Použití

`kolik command`

- příkazy:
  - `test <PATH> [options]`
    - otestuje soubor/program
  - `help`
    - zobrazí nápovědu

### Argumenty příkazu test

| Argument                 | Hodnota                                                                        |
| ------------------------ | ------------------------------------------------------------------------------ |
| `--compare-file <PATH>`  | cesta k souboru obsahující input/output pro program                            |
| `--test-separator <SEP>` | oddělovač jednotlivých testů                                                   |
| `--separator <SEP>`      | oddělovač vstupu a výstupu (výchozí hodnota je '---')                          |
| `--timeout <N>`          | maximální čas jednoho testu v milisekundách (výchozí hodnota je 10s)           |
| `--lang <LANG>`          | specifikuje o jaký typ souboru/programu se jedná (výchozí hodnota je "Python") |
| `--quiet`                | nevypíše výsledky test/ů do terminálu                                    |
| `--save <PATH>`                | specifikuje kam se mají uložit výsledky |

- pokud není argument `--save` nastaven, tak výsledky testů budou vypsány do terminálu v případě, že není nastaven argument `--quiet`