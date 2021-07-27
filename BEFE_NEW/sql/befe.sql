--
-- PostgreSQL database dump
--

-- Dumped from database version 9.6beta3
-- Dumped by pg_dump version 9.6beta3

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: bruce; Type: SCHEMA; Schema: -; Owner: bruce
--

CREATE SCHEMA bruce;


ALTER SCHEMA bruce OWNER TO bruce;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = bruce, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: t; Type: TABLE; Schema: bruce; Owner: bruce
--

CREATE TABLE t (
    id bigint NOT NULL,
    line text
);


ALTER TABLE t OWNER TO bruce;

--
-- Name: t_id_seq; Type: SEQUENCE; Schema: bruce; Owner: bruce
--

CREATE SEQUENCE t_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE t_id_seq OWNER TO bruce;

--
-- Name: t_id_seq; Type: SEQUENCE OWNED BY; Schema: bruce; Owner: bruce
--

ALTER SEQUENCE t_id_seq OWNED BY t.id;


--
-- Name: t id; Type: DEFAULT; Schema: bruce; Owner: bruce
--

ALTER TABLE ONLY t ALTER COLUMN id SET DEFAULT nextval('t_id_seq'::regclass);


--
-- Data for Name: t; Type: TABLE DATA; Schema: bruce; Owner: bruce
--

COPY t (id, line) FROM stdin;
1	Hi there
8	hi there again
\.


--
-- Name: t_id_seq; Type: SEQUENCE SET; Schema: bruce; Owner: bruce
--

SELECT pg_catalog.setval('t_id_seq', 8, true);


--
-- Name: t_id_index; Type: INDEX; Schema: bruce; Owner: bruce
--

CREATE UNIQUE INDEX t_id_index ON t USING btree (id);


--
-- PostgreSQL database dump complete
--

