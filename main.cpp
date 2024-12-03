#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <locale.h>
#include <map>
#include "univibe.hpp"

using namespace std;

int main(){

    string emailLogin;

    string listaUsuarios = "./usuarios.txt";
    string listaMusicas = "./musicas.txt";
    string listaPlay = "./playlist.txt";

    vector<Usuario> usuarios;
    vector<Musica> musicas;
    vector<Playlist> playlists;

    bool login = false;
    bool isArtista = false;

    
    while(true){

        usuarios = getUsuario(listaUsuarios);
        atualizarOuvintesUsuarios(listaUsuarios, usuarios);

        int inicio = menuLogin();

        if(inicio == 0){
            cout << "saindo..." << endl;
            break;
        }
        if(inicio == 1){
            login = usuarioLogin(usuarios, emailLogin);
            if (login) {
                break;
            }
        }
        if(inicio == 2){
            registrarUsuario(listaUsuarios);
        }
    }

    while(login){

        musicas = getMusica(listaMusicas);
        usuarios = getUsuario(listaUsuarios);
        playlists = getPlaylist(listaPlay);
        atualizarOuvintesUsuarios(listaUsuarios, usuarios);

        system("cls");

        imprimirUsuario(emailLogin, usuarios);
        
        bool isArtista = false;

        for (const auto& usuario : usuarios) {
            if (toLowerCase(usuario.email) == toLowerCase(emailLogin)) {
                isArtista = (usuario.artista == 's');
                break;
            }
        }
        
        cout << "\n--- MENU PRINCIPAL ---\n";
        cout << "1. Pesquisar musicas\n";
        cout << "2. Listar musicas por artista\n";
        cout << "3. Listar musicas por estilo\n";
        cout << "4. Criar playlist aleatoria por estilo\n";
        cout << "5. listar playlist\n";

        if(isArtista){
            cout << "6. Cadastrar musica\n";
            cout << "7. Editar musicas\n";
        }
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        
        int opcao;
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
            case 1:{
                system("cls");
                Musica musicaEncontrada = getMusicaByNome(musicas);
                if(!musicaEncontrada.nome.empty()){
                    cout << "Musica encontrada: " << musicaEncontrada.nome << " - " << musicaEncontrada.artista << endl;
                }
                break;
            }

            case 2:{
                system("cls");
                listaMusicasByArtista(musicas);
                break;
            }

            case 3:{
                system("cls");
                listaMusicasByEstilo(musicas);
                break;
            }

            case 4: {
                system("cls");
                auto playlist = playlistAleatoriaByEstilo(musicas, emailLogin);
                if (!playlist.empty()) {
                    cout << "--- Playlist Criada ---\n";
                    for (auto &musica : playlist) {
                        cout << "Musica: " << musica.nome << " | Artista: " << musica.artista << endl;
                    }
                }
                break;
            }

            case 5: {
                system("cls");
                playlists = getPlaylist(listaPlay);
                cout << "--- Listar Playlists ---\n";
                imprimirPlaylist(emailLogin, playlists);
                break;
            }

            case 6:{
                if (isArtista) {
                    system("cls");
                    cadastrarMusica(listaMusicas, usuarios, emailLogin);
                    musicas = getMusica(listaMusicas);  // Atualiza a lista de músicas após o cadastro
                } else {
                    cout << "Você nao tem permissao para cadastrar musicas." << endl;
                }
                break;
            }

            case 7:{
                if (isArtista) {
                    system("cls");
                    editarMusica(listaMusicas, musicas, usuarios, emailLogin);
                } else {
                    cout << "Você nao tem permissao para editar musicas." << endl;
                }
                break;
            }
            
            case 0:{
                system("cls");
                login = false;  // Encerra a sessão
                atualizarOuvintesUsuarios(listaUsuarios, usuarios);
                cout << "Logout realizado com sucesso!" << endl;
                break;
            }
            default:{
                cout << "Opcao invalida! Tente novamente.\n";
            }
        }
        
        if (opcao == 0) {
            break;
        }

        cout << "\nDeseja continuar no sistema? (s/n): ";
        char continuar;
        cin >> continuar;
        if (tolower(continuar) == 'n') {
            break;
        }
    }

    return 0;
}