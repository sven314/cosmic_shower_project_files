Bereinigt output von compare oder other_compare von solchen Situationen: (1 1,5 2) und (1,5 2) gefunden. Beachtet den unwahrscheinlichen Fall, 
dass Treffer identisch scheinen, aber nicht sind, da sie von verschiedenen Stationen kommen


Beispiel fuer erwartete Eingabe mit Stationsnummer und Timestamp:

4 502047690.000000000 5 502047691.000000000 6 502047692.000000000
1 502047690.000000000 2 502047691.000000000 1 502047692.000000000
5 502047691.000000000 6 502047692.000000000
2 502047691.000000000 1 502047692.000000000 
1 502047692.000000000 2 502047694.000000000
4 502047694.000000000 5 502047695.000000000 6 502047696.000000000


Treffer muessen nach dem ersten Timestamp aufsteigend sortiert sein. (Das wird  geprueft und korrigiert.)
Innere Treffer stehen dann nach dem aeusseren, da der Algorithmus das so ausspuckt.

Es ist noch notwendig, dass im aeusseren Treffer kein Event enthalten ist, das nach den letzten des inneren Treffers kommt. Beispiel:

5 502047691.000000000 6 502047692.000000000 wird nicht in
4 502047690.000000000 5 502047691.000000000 6 502047692.000000000 7 502047693.000000000 gefunden. 

Das Problem ist gering, da solche Situationen vom Algorithmus nicht erzeugt werden.

Bugs:

Der Vergleich ist nicht genau genug, geringe Unterschiede werden uebersehen
 
