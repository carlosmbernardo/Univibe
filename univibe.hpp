#ifndef UNIVIBE_HPP
#define UNIVIBE_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <locale.h>

using namespace std;


struct Musica{
    string artista = "";
    string nome = "";
    string estilo = "";
    long long int visualizacao = 0;
    long long int download = 0;
};

struct Usuario{
    string nome = "";
    string email= "";
    long long int ouvintes = 0;
    char artista;
};

struct Playlist{
    string email;
    string nomePlaylist;
    string nomeMusica;
    string estilo;
};

string abreviarNumero(long long int num);
string toLowerCase(string palavra);
void registrarUsuario(string enderecoArquivo);
void atualizarOuvintesUsuarios(string enderecoArquivo, vector<Usuario> usuarios);
vector<Musica> getMusica(string enderecoArquivo);
vector<Playlist> getPlaylist(string enderecoArquivo);
vector<Usuario> getUsuario(string enderecoArquivo);
Usuario getUsuarioByEmail(vector<Usuario> usuarios, string email);
Musica getMusicaByNome(vector<Musica> musicas);
void cadastrarMusica(string enderecoArquivoMusica, vector<Usuario> usuarios, string emailLogin);
void listaMusicasByArtista(vector<Musica> musicas);
void listaMusicasByEstilo(vector<Musica> musicas);
vector<Musica> playlistAleatoriaByEstilo(vector<Musica> musicas, string emailLogin);
int menuLogin();
bool usuarioLogin(vector<Usuario> usuarios, string &emailUsuario);
void imprimirPlaylist(string emailLogin, vector<Playlist> playlists);
void imprimirUsuario(string emailLogin, vector<Usuario> usuarios);
void editarMusica(string enderecoArquivoMusica, vector<Musica> &musicas, vector<Usuario> usuarios, string emailLogin);




#endif