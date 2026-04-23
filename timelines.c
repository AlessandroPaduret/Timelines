/*
 * timelines.c — Interfaccia C per il DB Timelines (Multiverso)
 * Compilazione:
 *   gcc timelines.c -o timelines -I/usr/include/postgresql -lpq
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

/* ──────────────────────────────────────────────────────────
 * Connessione
 * ────────────────────────────────────────────────────────── */

#define CONN_STR "host=localhost dbname=Timelines user=postgres password=TUA_PASSWORD"

/* Apre la connessione e termina il programma in caso di errore */
PGconn *db_connect(void) {
    PGconn *conn = PQconnectdb(CONN_STR);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Errore connessione: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(EXIT_FAILURE);
    }
    printf("Connesso al DB Timelines\n\n");
    return conn;
}

/* ──────────────────────────────────────────────────────────
 * Helper: stampa un risultato tabellare
 * ────────────────────────────────────────────────────────── */

void stampa_risultato(PGresult *res) {
    int ncols = PQnfields(res);
    int nrows = PQntuples(res);

    /* intestazione */
    for (int c = 0; c < ncols; c++) {
        printf("%-25s", PQfname(res, c));
    }
    printf("\n");
    for (int c = 0; c < ncols * 25; c++) printf("-");
    printf("\n");

    /* righe */
    for (int r = 0; r < nrows; r++) {
        for (int c = 0; c < ncols; c++) {
            printf("%-25s", PQgetvalue(res, r, c));
        }
        printf("\n");
    }
    printf("(%d righe)\n\n", nrows);
}

/* ──────────────────────────────────────────────────────────
 * 1. Elenca tutti gli universi
 * ────────────────────────────────────────────────────────── */

void elenca_universi(PGconn *conn) {
    printf("=== UNIVERSI ===\n");
    PGresult *res = PQexec(conn,
        "SELECT id, nome, descrizione FROM universo ORDER BY id;");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Errore query: %s\n", PQerrorMessage(conn));
    } else {
        stampa_risultato(res);
    }
    PQclear(res);
}

/* ──────────────────────────────────────────────────────────
 * 2. Elenca tutti i personaggi con le loro varianti
 * ────────────────────────────────────────────────────────── */

void elenca_personaggi_varianti(PGconn *conn) {
    printf("=== PERSONAGGI E VARIANTI ===\n");
    PGresult *res = PQexec(conn,
        "SELECT p.nome AS personaggio, v.nome_variante, u.nome AS universo "
        "FROM personaggio p "
        "JOIN variante v ON p.nome = v.personaggio_id "
        "JOIN universo u ON v.universo_id = u.id "
        "ORDER BY p.nome, v.nome_variante;");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Errore query: %s\n", PQerrorMessage(conn));
    } else {
        stampa_risultato(res);
    }
    PQclear(res);
}

/* ──────────────────────────────────────────────────────────
 * 3. Elenca tutti i viaggi (query 1 del README)
 * ────────────────────────────────────────────────────────── */

void elenca_viaggi(PGconn *conn) {
    printf("=== VIAGGI ===\n");
    PGresult *res = PQexec(conn,
        "SELECT v.nome_variante AS chi, p.nome AS eroe_base, "
        "       u_o.nome AS viene_da, u_d.nome AS atterrato_in, "
        "       vi.data_partenza, vi.data_arrivo "
        "FROM partecipazione part "
        "JOIN variante v    ON part.variante_id = v.id "
        "JOIN personaggio p ON v.personaggio_id = p.nome "
        "JOIN viaggio vi    ON part.viaggio_id  = vi.id "
        "JOIN universo u_o  ON v.universo_id          = u_o.id "
        "JOIN universo u_d  ON vi.universo_arrivo_id  = u_d.id;");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Errore query: %s\n", PQerrorMessage(conn));
    } else {
        stampa_risultato(res);
    }
    PQclear(res);
}

/* ──────────────────────────────────────────────────────────
 * 4. Chi ha viaggiato verso un universo specifico?
 *    Usa una query parametrizzata (sicura, no SQL injection)
 * ────────────────────────────────────────────────────────── */

void viaggiatori_verso(PGconn *conn, const char *universo_target) {
    printf("=== VIAGGIATORI VERSO '%s' ===\n", universo_target);

    const char *query =
        "SELECT DISTINCT p.nome, p.descrizione, u_d.nome AS destinazione "
        "FROM personaggio p "
        "JOIN variante v     ON p.nome = v.personaggio_id "
        "JOIN partecipazione pa ON pa.variante_id = v.id "
        "JOIN viaggio vi     ON pa.viaggio_id = vi.id "
        "JOIN universo u_d   ON vi.universo_arrivo_id = u_d.id "
        "WHERE u_d.nome = $1;";

    const char *params[1] = { universo_target };

    PGresult *res = PQexecParams(conn, query,
        1,       /* numero parametri */
        NULL,    /* tipi (NULL = inferiti da Postgres) */
        params,
        NULL, NULL,
        0        /* risultati in formato testo */
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Errore query: %s\n", PQerrorMessage(conn));
    } else {
        stampa_risultato(res);
    }
    PQclear(res);
}

/* ──────────────────────────────────────────────────────────
 * 5. Inserisce un nuovo personaggio
 * ────────────────────────────────────────────────────────── */

void inserisci_personaggio(PGconn *conn,
                           const char *nome,
                           const char *descrizione) {
    const char *query =
        "INSERT INTO personaggio (nome, descrizione) VALUES ($1, $2);";

    const char *params[2] = { nome, descrizione };

    PGresult *res = PQexecParams(conn, query, 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Errore INSERT personaggio: %s\n", PQerrorMessage(conn));
    } else {
        printf("Personaggio '%s' inserito con successo.\n\n", nome);
    }
    PQclear(res);
}

/* ──────────────────────────────────────────────────────────
 * 6. Inserisce una nuova variante
 * ────────────────────────────────────────────────────────── */

void inserisci_variante(PGconn *conn,
                        const char *nome_variante,
                        const char *personaggio_id,
                        int universo_id) {
    char universo_str[16];
    snprintf(universo_str, sizeof(universo_str), "%d", universo_id);

    const char *query =
        "INSERT INTO variante (nome_variante, personaggio_id, universo_id) "
        "VALUES ($1, $2, $3);";

    const char *params[3] = { nome_variante, personaggio_id, universo_str };

    PGresult *res = PQexecParams(conn, query, 3, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Errore INSERT variante: %s\n", PQerrorMessage(conn));
    } else {
        printf("Variante '%s' inserita con successo.\n\n", nome_variante);
    }
    PQclear(res);
}

/* ──────────────────────────────────────────────────────────
 * 7. Transazione: crea un viaggio + partecipazione atomicamente
 * ────────────────────────────────────────────────────────── */

void crea_viaggio(PGconn *conn,
                  int universo_partenza, int universo_arrivo,
                  const char *data_partenza, const char *data_arrivo,
                  int variante_id) {

    /* Inizio transazione */
    PGresult *res = PQexec(conn, "BEGIN;");
    PQclear(res);

    /* Inserisce il viaggio, ritorna l'id generato */
    char up[16], ua[16], vid[16];
    snprintf(up,  sizeof(up),  "%d", universo_partenza);
    snprintf(ua,  sizeof(ua),  "%d", universo_arrivo);
    snprintf(vid, sizeof(vid), "%d", variante_id);

    const char *q_viaggio =
        "INSERT INTO viaggio (universo_partenza_id, universo_arrivo_id, "
        "                     data_partenza, data_arrivo) "
        "VALUES ($1, $2, $3, $4) RETURNING id;";

    const char *p_viaggio[4] = { up, ua, data_partenza, data_arrivo };

    res = PQexecParams(conn, q_viaggio, 4, NULL, p_viaggio, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Errore INSERT viaggio: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }

    /* Recupera l'id appena inserito */
    char viaggio_id_str[16];
    strncpy(viaggio_id_str, PQgetvalue(res, 0, 0), sizeof(viaggio_id_str));
    PQclear(res);

    /* Inserisce la partecipazione */
    const char *q_part =
        "INSERT INTO partecipazione (variante_id, viaggio_id) VALUES ($1, $2);";
    const char *p_part[2] = { vid, viaggio_id_str };

    res = PQexecParams(conn, q_part, 2, NULL, p_part, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Errore INSERT partecipazione: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }
    PQclear(res);

    /* Commit */
    res = PQexec(conn, "COMMIT;");
    PQclear(res);

    printf("Viaggio %s creato (variante %d).\n\n", viaggio_id_str, variante_id);
}

/* ──────────────────────────────────────────────────────────
 * main — dimostra tutte le funzioni
 * ────────────────────────────────────────────────────────── */

int main(void) {
    PGconn *conn = db_connect();

    /* READ */
    elenca_universi(conn);
    elenca_personaggi_varianti(conn);
    elenca_viaggi(conn);
    viaggiatori_verso(conn, "Terra-1610");

    /* WRITE — decommenta per testare */
    /*
    inserisci_personaggio(conn,
        "Spider-Man 2099",
        "Miguel O'Hara, Spider-Man del futuro");

    inserisci_variante(conn,
        "Miguel O'Hara", "Spider-Man 2099", 928);

    crea_viaggio(conn,
        928, 616,
        "14:00:00", "14:08:00",
        7);   // variante_id = 7
    */

    PQfinish(conn);
    return EXIT_SUCCESS;
}