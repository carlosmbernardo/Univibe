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



string abreviarNumero(long long int num) {
    ostringstream oss;
    oss.precision(1);

    if (num >= 1e9) {
        oss << fixed << (num / 1e9) << " B";  // Bilhões
    } else if (num >= 1e6) {
        oss << fixed << (num / 1e6) << " M";  // Milhões
    } else if (num >= 1e3) {
        oss << fixed << (num / 1e3) << " K";  // Milhares
    } else {
        oss << num;  // Se for menor que mil, imprime normalmente
    }

    return oss.str();
}

string toLowerCase(string palavra){
    
    string palavraMinuscula;
    
    for(char c: palavra){
        palavraMinuscula += tolower(c);
    }
    
    return palavraMinuscula;
}

void registrarUsuario(string enderecoArquivo){
    ofstream arquivo(enderecoArquivo, ios::app);
    if (!arquivo.is_open()) {
        cout << "Arquivo não encontrado.";
    }
    string nome, email;
    //double seguidores = 0;
    double ouvintes = 0; //long ouvintes = 0;
    string artista;

    system("cls");

    cout << "***registrarUsuario***" << endl << endl;

    cout << "   nome do usuario: ";
    cin.ignore();
    getline(cin, nome);
    cout << "   email do usuario: ";
    //cin.ignore();
    getline(cin, email);
    cout << "   a conta eh de artista? " << endl;
    cout << "   sua resposta [s/n]: ";
    cin >> artista;
    artista = tolower(artista[0]);

    arquivo << nome << ',' << email << ',' << ouvintes << ',' << artista << endl;

    cout << "\ncadastrado com sucesso!" << endl;

}

void atualizarOuvintesUsuarios(string enderecoArquivo, vector<Usuario> usuarios) {
    ofstream arquivo(enderecoArquivo, ios::trunc); // Abre o arquivo para sobrescrever
    if (!arquivo.is_open()) {
        cout << "Arquivo de usuários não encontrado." << endl;
        return;
    }

    for (const Usuario usuario : usuarios) {
        arquivo << usuario.nome << "," << usuario.email << "," << usuario.ouvintes << "," << usuario.artista << endl;
    }

    arquivo.close();
}

vector<Musica> getMusica(string enderecoArquivo){
    
    vector<Musica> musicas;

    ifstream arquivo(enderecoArquivo);
    if (!arquivo.is_open()) {
        cout << "Arquivo nao encontrado.";
    }

    string linha;

    while(getline(arquivo, linha)){
        istringstream iss(linha);
        string token;
        vector<string> dadosMusica;

        while(getline(iss,token,',')){
            dadosMusica.push_back(token);
        }

        Musica musica;
        if(dadosMusica.size() == 5){
            musica.artista = dadosMusica[0];
            musica.nome = dadosMusica[1];
            musica.estilo = dadosMusica[2];
            musica.visualizacao = stoll(dadosMusica[3]);
            musica.download = stoll(dadosMusica[4]);

        musicas.push_back(musica);
        }
    }

    arquivo.close();
    return musicas;
}

vector<Playlist> getPlaylist(string enderecoArquivo){
    vector<Playlist> playlists;
    ifstream arquivo(enderecoArquivo);
    if (!arquivo.is_open()) {
        cout << "Arquivo nao encontrado.";
        return playlists; // Retorna um vetor vazio se o arquivo não abrir
    }

    string linha;
    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        string token;
        vector<string> dadosPlay;

        while (getline(iss, token, ',')) {
            dadosPlay.push_back(token);
        }

        if (dadosPlay.size() == 4) { // Verifica se tem todos os dados necessários
            Playlist playlist;
            playlist.email = dadosPlay[0];
            playlist.nomePlaylist = dadosPlay[1];
            playlist.nomeMusica = dadosPlay[2];
            playlist.estilo = dadosPlay[3];
            playlists.push_back(playlist);
        }
    }
    arquivo.close();
    return playlists;
}

vector<Usuario> getUsuario(string enderecoArquivo){
    vector<Usuario> usuarios;
    map<string, double> ouvintesPorArtista;

    ifstream arquivoMusicas("./musicas.txt");
    if (!arquivoMusicas.is_open()) {
        cout << "Arquivo de músicas não encontrado.";
        return usuarios; // Retorna um vetor vazio se o arquivo não abrir
    }

    string linha;
    while (getline(arquivoMusicas, linha)) {
        istringstream iss(linha);
        string token;
        vector<string> dadosMusica;

        while (getline(iss, token, ',')) {
            dadosMusica.push_back(token);
        }

        if (dadosMusica.size() == 5) {
            string artista = dadosMusica[0];
            long long int visualizacao = stoll(dadosMusica[3]);
            ouvintesPorArtista[toLowerCase(artista)] += visualizacao; // Soma as visualizações
        }
    }
    arquivoMusicas.close();
    
    ifstream arquivoUsuarios(enderecoArquivo);
    if (!arquivoUsuarios.is_open()) {
        cout << "Arquivo de usuários não encontrado.";
        return usuarios; // Retorna um vetor vazio se o arquivo não abrir
    }

    while (getline(arquivoUsuarios, linha)) {
        istringstream iss(linha);
        string token;
        vector<string> dadosUsuario;

        while (getline(iss, token, ',')) {
            dadosUsuario.push_back(token);
        }

        Usuario usuario;
        if (dadosUsuario.size() == 4) {
            usuario.nome = dadosUsuario[0];
            usuario.email = dadosUsuario[1];
            usuario.ouvintes = ouvintesPorArtista[toLowerCase(dadosUsuario[0])]; // Atribui os ouvintes
            usuario.artista = dadosUsuario[3][0];

            usuarios.push_back(usuario);
        }
    }

    arquivoUsuarios.close();
    return usuarios;
}

Usuario getUsuarioByEmail(vector<Usuario> usuarios, string email){
    
    Usuario usuarioEncontrado;

    for(Usuario usuario: usuarios){
        if(toLowerCase(usuario.email) == toLowerCase(email)){
            usuarioEncontrado = usuario;
            break;
        }
    }
    return usuarioEncontrado;
}

void cadastrarMusica(string enderecoArquivoMusica, vector<Usuario> usuarios, string emailLogin){
    
    ofstream arquivo(enderecoArquivoMusica, ios::app);
    if (!arquivo.is_open()) {
        cout << "Arquivo nao encontrado.";
    }

    string nome;
    Usuario artista = getUsuarioByEmail(usuarios, emailLogin);


    string nomeMusica;
    string estilo;
    double visualizacao = 0;
    double download = 0;

    cout << "***Cadastrando musica***" << endl << endl;
    cout << "insira o nome da musica: ";
    getline(cin, nomeMusica);
    cout << "insira o estilo da musica: ";
    getline(cin, estilo);

    arquivo << artista.nome << "," << nomeMusica << "," << estilo << "," << visualizacao << "," << download << endl;

    arquivo.close();
}

Musica getMusicaByNome(vector<Musica> musicas){
    
    Musica musicaEncontrada;
    string nome;

    cout << "insira o nome que deseja procurar: ";
    getline(cin, nome);
    
    bool encontrada = false;
    for(Musica musica: musicas){
        if(toLowerCase(musica.nome) == toLowerCase(nome)){
            musicaEncontrada = musica;
            encontrada = true;
            break;
        }
    }
    if(!encontrada){
        cout << "Musica nao encontrada !" << endl;
    }
    return musicaEncontrada;
}

void listaMusicasByArtista(vector<Musica> musicas){
    
    vector<Musica> musicasEncontradas;
    string nome;

    cout << "insira o nome do artista que deseja procurar: ";
    getline(cin, nome);

    for (Musica musica : musicas) {
        if (toLowerCase(musica.artista) == toLowerCase(nome)) { // if (musica.artista == nome) {
            musicasEncontradas.push_back(musica);  // Adiciona a música ao vetor de encontradas
        }
    }
    // Exibe as músicas encontradas
    if (!musicasEncontradas.empty()) {
        cout << "--Lista de musicas de " << nome << "--\n";
        for (Musica musica : musicasEncontradas) {
            cout << "Musica: " << musica.nome << " | Visualizacoes: " 
                 << abreviarNumero(musica.visualizacao) << " | Downloads: " 
                 << abreviarNumero(musica.download) << endl;
        }
    } else {
        cout << "Nenhuma musica encontrada para o artista: " << nome << endl;
    }
}

void listaMusicasByEstilo(vector<Musica> musicas){
    
    vector<Musica> musicasEncontradas;
    string estilo;

    cout << "insira o genero musical que deseja procurar: ";
    getline(cin, estilo);

    for (Musica musica : musicas) {
        if (toLowerCase(musica.estilo) == toLowerCase(estilo)) { // if(musica.estilo == estilo)
            musicasEncontradas.push_back(musica);  // Adiciona a música ao vetor de encontradas
        }
    }
    // Exibe as músicas encontradas
    if (!musicasEncontradas.empty()) {
        cout << "--Lista de musicas de " << estilo << "--\n";
        for (Musica musica : musicasEncontradas) {
            cout << "Musica: " << musica.nome << " | Artista: "
                 << musica.artista << " | Visualizacoes: " << abreviarNumero(musica.visualizacao)
                 << " | Downloads: " << abreviarNumero(musica.download) << endl;
        }
    } else {
        cout << "Nenhuma música encontrada para o estilo: " << estilo << endl;
    }
}

vector<Musica> playlistAleatoriaByEstilo(vector<Musica> musicas, string emailLogin)
{
    string estiloSelecionado;
    int tamanhoPlaylist;
    vector<Musica> playlist;          // Vetor para armazenar as playlists criadas pelo usuario
    vector<Musica> musicasInPlaylist; // vetor para armazenar as musicas na playlist

    cout << "Para qual estilo musical deseja criar sua playlist aleatoria? ";
    getline(cin, estiloSelecionado);
    cout << "Qual o nome da sua playlist de " << estiloSelecionado << "? ";
    string nomePlaylist;
    getline(cin, nomePlaylist);
    cout << "Quantas musicas deseja adicionar a sua playlist? ";
    cin >> tamanhoPlaylist;

    for (Musica musica : musicas)
    {
        if (toLowerCase(musica.estilo) == toLowerCase(estiloSelecionado))
        {
            musicasInPlaylist.push_back(musica);
        }
    }

    if (musicasInPlaylist.empty())
    {
        cout << "Nenhuma musica encontrada com o estilo selecionado!\n";
        return playlist;
    }

    srand(time(NULL));

    // Gerar a playlist aleatória
    for (int i = 0; i < tamanhoPlaylist && !musicasInPlaylist.empty(); ++i)
    {
        int indiceAleatorio = rand() % musicasInPlaylist.size();
        playlist.push_back(musicasInPlaylist[indiceAleatorio]);
        musicasInPlaylist.erase(musicasInPlaylist.begin() + indiceAleatorio); // Remove a música selecionada para evitar repetição
    }

    ofstream arquivo("./playlist.txt", ios::app);
    if (!arquivo.is_open()) {
        cout << "Arquivo de playlist nao encontrado." << endl;
    }
    for(Musica musica: playlist){
        arquivo << emailLogin << "," << nomePlaylist << "," << musica.nome << "," << toLowerCase(estiloSelecionado) << endl;
    }

    return playlist;
}

int menuLogin(){

    system("cls");
    int escolha;

    cout << "Bem vindo ao Univibe" << endl;
    cout << "- Entrar -" << endl << endl;
    cout << "   1- Logar" << endl;
    cout << "   2- Criar conta" << endl;
    cout << "\n   0- Sair" << endl;

    cout << "sua escolha: ";

    cin >> escolha;

    return escolha;
}

bool usuarioLogin(vector<Usuario> usuarios, string &emailUsuario){
    system("cls");
    string email;
    bool usuarioLogado = false;
    
    cout << "Login:" << endl << endl;
    cout << "insira seu email: ";
    cin.ignore();
    getline(cin, email);

    for(Usuario usuario: usuarios){
        if(toLowerCase(usuario.email) == toLowerCase(email)){
            cout << "Login efetuado com sucesso" << endl;
            emailUsuario = email; 
            usuarioLogado = true;
            Sleep(1000);
        }
    }
    if(!usuarioLogado){
        cout << "Email invalido" << endl;
        Sleep(1000);
    }
    return usuarioLogado;
}

void imprimirPlaylist(string emailLogin, vector<Playlist> playlists) {
    cout << "Qual o nome da playlist que quer procurar? ";
    string nomePlaylist;
    getline(cin, nomePlaylist);

    bool found = false;

    for (Playlist playlist : playlists){
        if (playlist.email == emailLogin && toLowerCase(playlist.nomePlaylist) == toLowerCase(nomePlaylist)){
            cout << "Playlist: " << playlist.nomePlaylist << " | Estilo: " << playlist.estilo << endl;
            cout << "Musicas:" << endl;


            for(Playlist playlist : playlists){
                if(playlist.email == emailLogin && toLowerCase(playlist.nomePlaylist) == toLowerCase(nomePlaylist)){
                    cout << " - " << playlist.nomeMusica << " (" << playlist.estilo << ")" << endl;
                }
            }

            found = true;
            break;
        }
    }

    if(!found){
        cout << "Playlist não encontrada" << endl;
    }
}

void imprimirUsuario(string emailLogin, vector<Usuario> usuarios){
    system("cls");
    
    for(Usuario usuario: usuarios){
        if(toLowerCase(usuario.email) == toLowerCase(emailLogin)){
            if(usuario.artista == 's'){
                cout << "   *** conta ARTISTA ***" << endl << endl;
                cout << "   " << usuario.nome << endl;
                cout << "   n' de ouvintes: " << abreviarNumero(usuario.ouvintes) << endl;
            }else{
                cout << "   *** conta PADRAO ***" << endl << endl;
                cout << "   " << usuario.nome << endl;
            }
        }
    }
}

void editarMusica(string enderecoArquivoMusica, vector<Musica> &musicas, vector<Usuario> usuarios, string emailLogin){
    bool musicaEncontrada = false;
    bool usuarioEncontrado = false;
    bool musicaEditavel = false;
    string nome, nomeMusica;
    
    cout << "nome da musica que deseja editar: " << endl;
    getline(cin, nomeMusica);
    
    for(Usuario usuario: usuarios){
        if(emailLogin == usuario.email){
            nome = usuario.nome; 
            usuarioEncontrado = true;
        }
    }
    
    for(Musica &musica : musicas){
        if(usuarioEncontrado && nome == musica.artista){
            if(toLowerCase(musica.nome) == toLowerCase(nomeMusica)){
                musicaEditavel = true;
                musicaEncontrada = true;
                cout << "Editar dados da musica: " << musica.nome << endl;
                cout << "Novo estilo: ";
                getline(cin, musica.estilo);
                cout << "Novas visualizacoes: ";
                cin >> musica.visualizacao;
                cout << "Novos downloads: ";
                cin >> musica.download;

                break;
            }
        }
    }
    if(!musicaEditavel){
        cout << "voce nao tem permissao de editar esta musica !" << endl;
        return;
    }

    if(musicaEncontrada){
        // Sobrescreve o arquivo com as mudanças feitas
        ofstream arquivo(enderecoArquivoMusica, ios::trunc);
        for(Musica musica : musicas){
            arquivo << musica.artista << "," << musica.nome << "," << musica.estilo << "," 
                    << musica.visualizacao << "," << musica.download << endl;
        }
        arquivo.close();
        cout << "Musica atualizada com sucesso!" << endl;
    } else {
        cout << "Musica não encontrada." << endl;
    }
}
