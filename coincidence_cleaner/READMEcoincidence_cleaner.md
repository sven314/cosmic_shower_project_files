Bereinigt output von compare oder other_compare von solchen Situationen: (1 1,5 2) und (1,5 2) gefunden. Beachtet den unwahrschjeinlichen Fall, dass Treffer identisch scheinen aber nicht sind, das sie von verschiedenen Stationen kommen


Beispiel fuer erwartete Eingabe: 
4 502047694.000000000 5 502047695.000000000 6 502047696.000000000
1 502047692.000000000 2 502047694.000000000
2 502047691.000000000 1 502047692.000000000 
5 502047691.000000000 6 502047692.000000000
1 502047690.000000000 2 502047691.000000000 1 502047692.000000000
4 502047690.000000000 5 502047691.000000000 6 502047692.000000000

Treffer muessen nach dem ersten Timestamp Absteigend sortiert sein. (Das wird noch nicht geprueft oder korrigiert.)
Innere Treffer stehen dann vor dem goesseren, da der Algorithmus das so ausspuckt.