#include "SceneManager.h"
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

BaseScene* SceneManager::m_pScene = nullptr; // nullptr で初期化

void SceneManager::ChangeScene(SCENE scene , int score) {
    if (m_pScene != nullptr) {
        delete m_pScene;
    }

    switch (scene) {
    case SCENE::TITLE:
        m_pScene = new TitleScene(); // タイトルシーンを設定
        break;
    case SCENE::GAME:
        m_pScene = new GameScene(score);  // ゲームシーンを設定
        break;
    case SCENE::RESULT:
        m_pScene = new ResultScene(score); // リザルトシーンを設定
        break;
    default:
        break;
    }
}

void SceneManager::Update() {
    if (m_pScene) {
        m_pScene->Update(); // 現在のシーンの更新
    }
}

void SceneManager::Render() {
    if (m_pScene) {
        m_pScene->Draw(); // 現在のシーンの描画
    }
}
