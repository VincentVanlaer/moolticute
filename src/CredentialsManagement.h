/******************************************************************************
 **  Copyright (c) Raoul Hecky. All Rights Reserved.
 **
 **  Moolticute is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Moolticute is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/
#ifndef CREDENTIALSMANAGEMENT_H
#define CREDENTIALSMANAGEMENT_H

// Qt
#include <QtWidgets>

// Application
#include "WSClient.h"
#include "TOTPCredential.h"

namespace Ui {
class CredentialsManagement;
}
class CredentialModel;
class CredentialModelFilter;
class LoginItem;
class PasswordProfilesModel;

class CredentialsManagement : public QWidget
{
    Q_OBJECT

    enum class LinkingMode {
        OFF,
        NEW_CREDENTIAL,
        CREDENTIAL_EDIT
    };

public:    
    explicit CredentialsManagement(QWidget *parent = 0);
    ~CredentialsManagement();
    void setWsClient(WSClient *c);
    void setPasswordProfilesModel(PasswordProfilesModel *passwordProfilesModel);
    bool isClean() const;

public slots:
    bool confirmDiscardUneditedCredentialChanges(const QModelIndex &proxyIndex = {});
    void saveChanges();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void enableCredentialsManagement(bool);
    void updateQuickAddCredentialsButtonState();
    void onPasswordInputReturnPressed();
    void updateSaveDiscardState(const QModelIndex &proxyIndex=QModelIndex());
    void onPasswordUnlocked(const QString &service, const QString &login, const QString &password, bool success);
    void onCredentialUpdated(const QString &service, const QString &login, const QString &description, bool success);
    void saveSelectedCredential();
    void saveSelectedTOTP();
    void on_pushButtonEnterMMM_clicked();
    void on_buttonDiscard_pressed();
    void onButtonDiscard_confirmed();
    void on_buttonSaveChanges_clicked();
    void requestPasswordForSelectedItem();
    void on_addCredentialButton_clicked();
    void on_pushButtonConfirm_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonDelete_clicked();
    void on_toolButtonClearFilter_clicked();
    void on_lineEditFilterCred_textChanged(const QString &text);
    void onCredentialSelected(const QModelIndex &current, const QModelIndex &previous);
    void onLoginSelected(const QModelIndex &srcIndex);
    void onServiceSelected(const QModelIndex &srcIndex);
    void onItemExpanded(const QModelIndex &proxyIndex);
    void onItemCollapsed(const QModelIndex &proxyIndex);
    void onExpandedStateChanged(bool bIsExpanded);
    void onModelLoaded(bool bClearLoginDescription);
    void onSelectLoginItem(LoginItem *pLoginItem);
    void onSelectLoginTimerTimeOut();
    void updateFavMenu();
    void credentialDataChanged();
    void checkDeviceType();
    void updateFavMenuOnDevChanged(Common::MPHwVersion newDev);
    void updateDeviceType(Common::MPHwVersion newDev);

    void on_toolButtonFavFilter_clicked();

    void on_toolButtonEditService_clicked();

    void on_pushButtonSaveCategories_clicked();
    void onCategoryEdited(const QString& edited);
    void handleAdvancedModeChange(bool isEnabled);

    void on_pushButtonTOTP_clicked();

    void on_pushButtonDeleteTOTP_clicked();

    void onCredentialLink();
    void onCredentialLinkRemoved();
    void onSelectedCredentialLink();
    void onSelectedCredentialLinkRemoved();

    void on_pushButtonDiscardLinking_clicked();

    void on_pushButtonLinkTo_clicked();

    void onTreeViewContextMenuRequested(const QPoint& pos);

private:
    void updateLoginDescription(const QModelIndex &srcIndex);
    void updateLoginDescription(LoginItem *pLoginItem);
    void clearLoginDescription();
    QModelIndex getSourceIndexFromProxyIndex(const QModelIndex &proxyIndex);
    QModelIndex getProxyIndexFromSourceIndex(const QModelIndex &srcIndex);
    void setCredentialsClean();

    void disableNonCredentialEditWidgets();
    void enableNonCredentialEditWidgets();
    bool isServiceNameExist(const QString& serviceName) const;
    bool isLoginNameExistForService(const QString& loginName, const QString& serviceName) const;
    void setServiceInputAttributes(const QString& tooltipText, Qt::GlobalColor col);
    void setLoginInputAttributes(const QString& tooltipText, Qt::GlobalColor col);
    void clearMMMUi();
    void updateBleFavs(const QModelIndex &srcIndex);
    void sendGetUserCategories();
    void initKeyAfterInput(QComboBox * cbKeyAfter);

    void changeCurrentFavorite(int iFavorite);
    virtual void changeEvent(QEvent *event);
    int getCategory(const QModelIndex &srcIndex);

    void checkLinkingOnLoginSelected(const QModelIndex &srcIndex);

    QString processMultipleDomainsInput(const QString& service, const QString &domains);

    bool isUICategoryClean() const;

    Ui::CredentialsManagement *ui;
    CredentialModel *m_pCredModel = nullptr;
    CredentialModelFilter *m_pCredModelFilter = nullptr;
    WSClient *wsClient = nullptr;
    QTimer m_tSelectLoginTimer;
    LoginItem *m_pAddedLoginItem;
    TOTPCredential *m_pTOTPCred = nullptr;
    QMenu m_enableMultipleDomainMenu;

    QMenu m_favMenu;
    QJsonArray m_loadedModelSerialiation;
    bool m_selectionCanceled;
    bool m_isClean = true;
    bool m_isSetCategoryClean = true;
    bool m_altKeyPressed = false;

    LinkingMode m_linkingMode = LinkingMode::OFF;
    QByteArray m_credentialLinkedAddr;
    QModelIndex m_credentialToLinkIndex;

    void saveCredential(const QModelIndex currentSelectionIndex);

    void setFilterCredLayout();
    
    LoginItem * tryGetSelectedLogin();


    static constexpr int MINI_FAVORITE_NUM = 14;
    static constexpr int BLE_FAVORITE_NUM = 50;
    static constexpr int MINI_PASSWORD_LENGTH = 31;
    static constexpr int BLE_PASSWORD_LENGTH = 64;
    static constexpr char MULT_DOMAIN_SEPARATOR = ',';

signals:
    void wantEnterMemMode();
    void wantSaveMemMode();
    void loginSelected(const QModelIndex &srcIndex);
    void serviceSelected(const QModelIndex &srcIndex);
    void selectLoginItem(const QModelIndex &proxyIndex);
    void newCredentialLinked();
    void editedCredentialLinked();
    void displayCredentialLink();
    void hideCredentialLink();
};

#endif // CREDENTIALSMANAGEMENT_H
