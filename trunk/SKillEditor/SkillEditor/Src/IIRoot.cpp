#include "IIRoot.h"

#include "renderwindow.h"

bool IIRoot::setup(QGraphicsView *graphicsView)
{
    bool rtn=false;
    mRoot = new Ogre::Root("Plugins.cfg","ogre.cfg","log.txt");
    if(!mRoot->restoreConfig())
    {
        rtn=mRoot->showConfigDialog();
    }
    else
    {
        rtn=true;
    }
    if (rtn)
    {
      mRoot->saveConfig();
      mRoot->initialise(false);

      Ogre::NameValuePairList params;
      params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)(HWND)graphicsView->winId());
      mWindow = mRoot->createRenderWindow("View",graphicsView->width(),graphicsView->height(), false, &params);

      if(!setupResourcesPath())
      {
          return false;
      }

	  mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
      mRgm=Ogre::ResourceGroupManager::getSingletonPtr();

      mCamera = mSceneMgr->createCamera("PlayerCam");
      mCamera->setPosition(Ogre::Vector3(0,20,100));
      mCamera->lookAt(Ogre::Vector3(0,0,0));
      mCamera->setNearClipDistance(5);
	  ((IIRenderWindow*)graphicsView)->setCamera(mCamera);

      Ogre::Viewport* vp = mWindow->addViewport(mCamera);
      mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

      mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	  mSceneMgr->setShadowTechnique(Ogre::SHADOWDETAILTYPE_STENCIL);
      mLight=mSceneMgr->createLight("MainLight");
	  mLight->setPosition(-500.0f,500.0f, 500.0f);
	  mLight->setDirection(1.0f, -1.0f, -1.0f);
	  mLight->setCastShadows(true);
	  mLight->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f,1.0f));

	  setupTerrain(513,1000,100,QString("grass_1024.jpg"),QString("waveNM.png"));

      return true;
    }
    else
    {
        return false;
    }
}

void IIRoot::setupTerrain(int TERRAIN_SIZE,int TERRAIN_WORLD_SIZE,int BlendMapWorldSize,QString DefaulTexture,QString DefaulNormalMap)
{

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

	//����ȫ������
	mTerrainGlobals = new Ogre::TerrainGlobalOptions();
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);
    mTerrainGlobals->setLightMapDirection(mLight->getDerivedDirection());//���ù��ߵķ������е��ο������ͬһ������Ĺ�
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());//���û�����
    mTerrainGlobals->setCompositeMapDiffuse(mLight->getDiffuseColour());//�����������
    mTerrainGlobals->setLightMapSize(TERRAIN_SIZE); //���ù���ͼ�Ĵ�С���������ԽС����ͼ����Ӱ��Խ�ֲ�

	//�������η�����󣬸ö������ڴ����Լ��������ο�
	mTerrainGroup = new Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);//���������������ε�ƽ�̷��򡢵��εĴ�С
    mTerrainGroup->setOrigin(Ogre::Vector3(0,0,0)); //�����˸õ��������ʼλ�ã����Ժ󴴽��ĵ��ο��о����ô�λ����Ϊ���λ��

	/*
	���˵��η���֮�����ǾͿ���ͨ�����η��鴴�����ο��ˣ�����ÿһ�����ο鶼�кܶ����ԣ����ǿ����ڴ������ο��ͬʱ������Щ���ԣ�����������Ϊ�����㡣���ԣ����ǿ���������Ĭ�ϵĵ��ο����ԣ���ô�������ο��ʱ��Ϳ���һ�������㶨��*/

    //���õ���Ĭ������
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;
    defaultimp.inputScale = 600;//��������
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    //�������� 
    defaultimp.layerList.resize(1);
    defaultimp.layerList[0].worldSize = BlendMapWorldSize;
	defaultimp.layerList[0].textureNames.push_back(DefaulTexture.toLatin1().data());//Ĭ����ͼ
	defaultimp.layerList[0].textureNames.push_back(DefaulNormalMap.toLatin1().data());//Ĭ�Ϸ�����ͼ

	mTerrainGroup->defineTerrain(0, 0,0.0f);
	mTerrainGroup->loadAllTerrains(true);

}

void IIRoot::setupLog(Ogre::LogListener* w)
{
    //logMgr = new Ogre::LogManager();
    //log=logMgr->createLog("mylog.log", true, false, true);
    //log->addListener(w);
    //logMgr->setLogDetail(Ogre::LL_NORMAL);
}

void IIRoot::setPathToFile(QString dir,QString secName)
{
        QFile resFile("setting.cfg");
         if (!resFile.open(QFile::ReadOnly | QFile::Text)) {
             QMessageBox::warning(0, "Scene Editor","Cannot open file");
             return;
         }
         QTextStream in(&resFile);

         QFile tempFile("temp.cfg");
         if (!tempFile.open(QFile::WriteOnly | QFile::Text)) {
             QMessageBox::warning(0, "Scene Editor","Cannot open file");
             return;
         }
         QTextStream out(&tempFile);

         while(!in.atEnd())
         {
             QString data=in.readLine();
             if(data=="["+secName+"]")
             {
                 out<<"["+secName+"]"<<endl;
                 out<<"Path="<<dir<<endl;
                 data=in.readLine();
             }
             else
             {
                 out<<data<<endl;
             }
         }
        resFile.close();
        resFile.remove();
		tempFile.close();
        tempFile.rename("temp.cfg","setting.cfg");
}

void IIRoot::setResourcePath(QDir* dir,QString secName)
{
    QFileInfoList fileList=dir->entryInfoList(QDir::Dirs);
    fileList.removeAt(1);//�Ƴ� ..
    if(!fileList.isEmpty())
    {
        for(int i=0;i<fileList.count();i++)
        {
                QString path=fileList.at(i).filePath();
                path.replace("/", "\\");
				Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(std::string(path.toLatin1().data()),"FileSystem",std::string(secName.toLatin1().data()));
                if(path.right(1)!=".")//��ֹ�����ݹ�
                {
                    setResourcePath(new QDir(fileList.at(i).filePath()),secName);
                }
        }
    }
//    delete dir;
}

bool IIRoot::setupResourcesPath()
{
    Ogre::ConfigFile cf;
    cf.load("setting.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
                QDir dir(*new QString(archName.c_str()));
                if(!dir.exists())
                {
                    QMessageBox::warning(0,"warning!","From setting.cfg's ResourceLib path inexistence.");
					QString resPath = QFileDialog::getExistingDirectory(0,QString("set ResourceLib path for ")+QString(secName.c_str()),QString(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
                    if(resPath.isNull())
                    {
                        return false;
                    }
                    dir.setPath(resPath);
                    setPathToFile(resPath,secName.c_str());
                }

                Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(secName);
                QString path=dir.path();
                path.replace("/", "\\");
                resPathList.insert(secName.c_str(),path);
                setResourcePath(&dir,secName.c_str());
        }
    }
    Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();
    return true;
}