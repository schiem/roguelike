#include <gui.h>
#define CHUNK_DIR DATADIR "/chunk"
//#include <boost/filesystem.hpp>

using namespace std;

//namespace fs=boost::filesystem;

int main(int argc, char* args[]) {
    cout<<CHUNK_DIR<<endl;

    //cout<<"Testing boost with /etc/fstab file size: "<<fs::file_size("/etc/fstab")<<endl;

    GUI theApp;
    return theApp.OnExecute();
}
