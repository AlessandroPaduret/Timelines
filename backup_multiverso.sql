--
-- PostgreSQL database dump
--

\restrict jcAlD5cS2Maf2Zban4mWovhCrgFE7K280OssdooWSfgrxiUjssz0guESrtoCsPS

-- Dumped from database version 18.3
-- Dumped by pg_dump version 18.3

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: personaggio; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.personaggio (
    nome character varying(100) NOT NULL,
    descrizione text
);


ALTER TABLE public.personaggio OWNER TO postgres;

--
-- Name: personaggio_viaggio; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.personaggio_viaggio (
    personaggio_id character varying(100) NOT NULL,
    viaggio_id integer NOT NULL
);


ALTER TABLE public.personaggio_viaggio OWNER TO postgres;

--
-- Name: universo; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.universo (
    id integer NOT NULL,
    nome character varying(100),
    descrizione text
);


ALTER TABLE public.universo OWNER TO postgres;

--
-- Name: variante; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.variante (
    id integer NOT NULL,
    personaggio_id character varying(100),
    universo_id integer
);


ALTER TABLE public.variante OWNER TO postgres;

--
-- Name: viaggio; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.viaggio (
    id integer NOT NULL,
    universo_partenza_id integer,
    universo_arrivo_id integer,
    data_partenza time without time zone,
    data_arrivo time without time zone
);


ALTER TABLE public.viaggio OWNER TO postgres;

--
-- Data for Name: personaggio; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.personaggio (nome, descrizione) FROM stdin;
Spider-Man	Eroe morso da un ragno radioattivo o geneticamente modificato
Spider-Gwen	Gwen Stacy morsa dal ragno al posto di Peter Parker
Spider-Ham	Un maiale con poteri ragneschi
\.


--
-- Data for Name: personaggio_viaggio; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.personaggio_viaggio (personaggio_id, viaggio_id) FROM stdin;
Spider-Man	100
Spider-Gwen	101
Spider-Man	102
\.


--
-- Data for Name: universo; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.universo (id, nome, descrizione) FROM stdin;
1610	Terra-1610	Universo di Miles Morales
616	Terra-616	Universo di Peter B. Parker
65	Terra-65	Universo di Gwen Stacy
90214	Terra-90214	Universo di Spider-Man Noir
14512	Terra-14512	Universo di Peni Parker
8311	Terra-8311	Universo di Spider-Ham
\.


--
-- Data for Name: variante; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.variante (id, personaggio_id, universo_id) FROM stdin;
1	Spider-Man	1610
2	Spider-Man	616
3	Spider-Gwen	65
4	Spider-Man	90214
5	Spider-Man	14512
6	Spider-Ham	8311
\.


--
-- Data for Name: viaggio; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.viaggio (id, universo_partenza_id, universo_arrivo_id, data_partenza, data_arrivo) FROM stdin;
100	616	1610	10:00:00	10:05:00
101	65	1610	09:30:00	09:35:00
102	90214	1610	00:00:00	10:10:00
\.


--
-- Name: personaggio personaggio_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.personaggio
    ADD CONSTRAINT personaggio_pkey PRIMARY KEY (nome);


--
-- Name: personaggio_viaggio personaggio_viaggio_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.personaggio_viaggio
    ADD CONSTRAINT personaggio_viaggio_pkey PRIMARY KEY (personaggio_id, viaggio_id);


--
-- Name: universo universo_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.universo
    ADD CONSTRAINT universo_pkey PRIMARY KEY (id);


--
-- Name: variante variante_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.variante
    ADD CONSTRAINT variante_pkey PRIMARY KEY (id);


--
-- Name: viaggio viaggio_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.viaggio
    ADD CONSTRAINT viaggio_pkey PRIMARY KEY (id);


--
-- Name: personaggio_viaggio personaggio_viaggio_personaggio_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.personaggio_viaggio
    ADD CONSTRAINT personaggio_viaggio_personaggio_id_fkey FOREIGN KEY (personaggio_id) REFERENCES public.personaggio(nome);


--
-- Name: personaggio_viaggio personaggio_viaggio_viaggio_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.personaggio_viaggio
    ADD CONSTRAINT personaggio_viaggio_viaggio_id_fkey FOREIGN KEY (viaggio_id) REFERENCES public.viaggio(id);


--
-- Name: variante variante_personaggio_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.variante
    ADD CONSTRAINT variante_personaggio_id_fkey FOREIGN KEY (personaggio_id) REFERENCES public.personaggio(nome);


--
-- Name: variante variante_universo_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.variante
    ADD CONSTRAINT variante_universo_id_fkey FOREIGN KEY (universo_id) REFERENCES public.universo(id);


--
-- Name: viaggio viaggio_universo_arrivo_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.viaggio
    ADD CONSTRAINT viaggio_universo_arrivo_id_fkey FOREIGN KEY (universo_arrivo_id) REFERENCES public.universo(id);


--
-- Name: viaggio viaggio_universo_partenza_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.viaggio
    ADD CONSTRAINT viaggio_universo_partenza_id_fkey FOREIGN KEY (universo_partenza_id) REFERENCES public.universo(id);


--
-- PostgreSQL database dump complete
--

\unrestrict jcAlD5cS2Maf2Zban4mWovhCrgFE7K280OssdooWSfgrxiUjssz0guESrtoCsPS

