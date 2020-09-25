#include "textureHolder.h"
#include <stdexcept>

// Ne treba mijenjati
TextureHolder::TextureHolder(){
    setFileNames();
    mTextures.resize(Textures::size);
    for(int i=0; i<Textures::size; ++i){
        mTextures[i].reset(new sf::Texture);
        if(!mTextures[i]->loadFromFile(mFileNames[i]))
            throw std::runtime_error("Cannot open file " + mFileNames[i]);
    }
}


void TextureHolder::setFileNames(){
    mFileNames.resize(Textures::size);

    mFileNames[Textures::background] = "bg5.jpg";
    mFileNames[Textures::player] = "playerShip3_blue.png"; 
    mFileNames[Textures::bullet] = "bullet.png";
    mFileNames[Textures::explosion] = "exp2_0.png";
    mFileNames[Textures::meteorBig1] = "meteorBrown_big1.png";
    mFileNames[Textures::meteorBig2] = "meteorBrown_big2.png";
    mFileNames[Textures::meteorBig3] = "meteorBrown_big3.png";
    mFileNames[Textures::meteorBig4] = "meteorBrown_big4.png";
    mFileNames[Textures::meteorMedium1] = "meteorBrown_med1.png";
    mFileNames[Textures::meteorMedium2] = "meteorBrown_med2.png";
    mFileNames[Textures::meteorSmall1] = "meteorBrown_small1.png";
    mFileNames[Textures::meteorSmall2] = "meteorBrown_small2.png";
}
