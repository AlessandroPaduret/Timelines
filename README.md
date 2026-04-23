
## Indice

###### Abstract
###### Requisiti
###### Schema Concettuale
###### Schema Logico


## Abstract

Si vuole creare un database per la gestione di un universo narrativo complesso, in cui i personaggi possono esistere in molteplici varianti (multiverso) e possono trovarsi in universi che non sono loro. L'obiettivo è quello di fornire agli autori uno strumento per mappare ramificazioni narrative infinite, garantendo che ogni variante di un personaggio sia coerente con la propria linea temporale d'origine e la loro posizione in universi diversi.

- TODO: gestione creazione linee temporali e legami tra di esse
- TODO: gestione eventi e loro impatto sui personaggi e sulle linee temporali


## Requisiti

Di ogni universo ci interessa: nome, descrizione.
Di ogni personaggio ci interessa: nome, descrizione.
Una variante è un personaggio che esiste in un universo specifico. 
Di ogni variante ci interessa: id, personaggio_id, universo_id.



Ci sono tanti universi identificati da un id. personaggi identificati da un nome e una descrizione.
ogni personaggio può esistere in più universi (varianti) e ogni variante è associata a un universo specifico.
ogni personaggio può viaggiare tra universi diversi.
di ogni viaggio ci interessano: data, universo di partenza, universo di arrivo, personaggio coinvolto, motivo del viaggio


## Schema concettuale

![schema concettuale](er.jpg)

## Schema logico


- Universo (**id**, nome, descrizione)
- Personaggio (**nome**, descrizione)
- Variante (**id**, nome_variante, personaggio_id, universo_id)
- Viaggio (**id**, universo_partenza_id, universo_arrivo_id, data_partenza,data_arrivo)
- Partecipazione (**variante_id**, **viaggio_id**)


nota: uso gli id perché altrimenti le tabelle diventano troppo grandi e portare chiavi coposte è scomodo


## Query di esempio 

```sql
-- 1. Elenca tutti i viaggi con i dettagli del personaggio, variante, universo di origine e universo di destinazione
SELECT
    V.nome_variante AS "Chi", 
    P.nome AS "Eroe Base",
    U_Origine.nome AS "Viene da", 
    U_Dest.nome AS "Atterrato in",
    Viag.data_partenza AS "Partito il",
	Viag.data_arrivo AS "Arrivato il"
FROM Partecipazione Part
JOIN Variante V ON Part.variante_id = V.id
JOIN Personaggio P ON V.personaggio_id = P.nome
JOIN Viaggio Viag ON Part.viaggio_id = Viag.id
JOIN Universo U_Origine ON V.universo_id = U_Origine.id
JOIN Universo U_Dest ON Viag.universo_arrivo_id = U_Dest.id;

-- 2. per ogni personaggio, il numero di viaggi effettuati dalla sua variante che ha fatto più viaggi
-- TO DO
```

ps. per esportare DB 
```bash 
# generale
pg_dump -U nome_utente -d database_name > backup_multiverso.sql
# quello che ho usato io
pg_dump -U postgres -d Timelines > backup_multiverso.sql
```# Timelines
