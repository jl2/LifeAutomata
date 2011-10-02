/*
  lifewindow.h
  
  Copyright (c) 2011, Jeremiah LaRocco jeremiah.larocco@gmail.com

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QtGui>
#include <stdexcept>
#include "lifewindow.h"

void LifeWindow::readSettings() {
  settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                           "Life", "Life");
    
}

LifeWindow::LifeWindow(QWidget *parent) : QMainWindow(parent) {
    readSettings();

    life = new LifeWidget;
    setCentralWidget(life);
    connect(life, SIGNAL(iterationDone(int)), this, SLOT(updateIteration(int)));

    loadPlugins();

    setupActions();
    setupToolBar();
    setupMenuBar();
    setupStatusBar();

    setWindowTitle(tr("Life"));
}

LifeWindow::~LifeWindow() {
    settings->sync();
}

void LifeWindow::setupActions() {
    startAction = new QAction(tr("Start"), this);
    // startAction->setIcon(QIcon(":/images/start.png"));
    startAction->setStatusTip(tr("Start iterating current life pattern"));
    connect(startAction, SIGNAL(triggered()), life, SLOT(start()));
    
    stopAction = new QAction(tr("Stop"), this);
    // stopAction->setIcon(QIcon(":/images/stop.png"));
    stopAction->setStatusTip(tr("Stop iterating current life pattern"));
    connect(stopAction, SIGNAL(triggered()), life, SLOT(stop()));

    // Exit
    exitAction = new QAction(tr("Exit"), this);
    // exitAction->setIcon(QIcon(":/images/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));//exit()));

    // Reset
    resetAction = new QAction(tr("Reset"), this);
    // resetAction->setIcon(QIcon(":/images/view.png"));
    resetAction->setStatusTip(tr("Restart"));
    connect(resetAction, SIGNAL(triggered()), life, SLOT(reset()));

    // Configure
    configureAction = new QAction(tr("Configure"), this);
    // configureAction->setIcon(QIcon(":/images/view.png"));
    configureAction->setStatusTip(tr("Restart"));
    connect(configureAction, SIGNAL(triggered()), this, SLOT(configureCurrentPlugin()));

    // Reset view
    resetViewAction = new QAction(tr("Reset View"), this);
    // resetViewAction->setIcon(QIcon(":/images/view.png"));
    resetViewAction->setStatusTip(tr("Reset the view"));
    connect(resetViewAction, SIGNAL(triggered()), life, SLOT(resetView()));

    // About
    aboutAction = new QAction(tr("About"), this);
    // aboutAction->setIcon(QIcon(":/images/about.png"));
    aboutAction->setStatusTip(tr("About this Life program"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

}

void LifeWindow::setupToolBar() {
    QToolBar *tb = new QToolBar("Main", this);
    tb->addAction(startAction);
    tb->addAction(stopAction);
    tb->addAction(resetAction);
    tb->addAction(resetViewAction);
    
    addToolBar(tb);
}

void LifeWindow::setupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(startAction);
    fileMenu->addAction(stopAction);
    fileMenu->addAction(resetAction);
    fileMenu->addAction(resetViewAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    pluginMenu = menuBar()->addMenu(tr("Type"));
    for (QMap<QString, LifePlugin *>::const_iterator iter = plugins.constBegin();
         iter != plugins.constEnd(); ++iter) {
        QAction *temp;
        temp = new QAction(iter.value()->name(), this);
        temp->setStatusTip(iter.value()->description());
        connect(temp, SIGNAL(triggered()), this, SLOT(pluginChanged()));
        pluginMenu->addAction(temp);
    }
    pluginMenu->addSeparator();
    pluginMenu->addAction(configureAction);
  
    menuBar()->addSeparator();
  
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void LifeWindow::configureCurrentPlugin() {
    plugins[curPlugin]->configure(this, settings);
}

void LifeWindow::setupStatusBar() {
    statusBar()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    curIterLabel = new QLabel;
    curIterLabel->setMaximumWidth(fontMetrics().maxWidth()*24);
    curIterLabel->setMinimumWidth(fontMetrics().maxWidth()*10);
    curIterLabel->setText(tr("Iteration: 0"));
    curIterLabel->setAlignment(Qt::AlignHCenter);

    curPluginLabel = new QLabel;
    curPluginLabel->setMaximumWidth(fontMetrics().maxWidth()*24);
    curPluginLabel->setMinimumWidth(fontMetrics().maxWidth()*10);
    curPluginLabel->setText(tr("Current Plugin: %1").arg(curPlugin));
    curPluginLabel->setAlignment(Qt::AlignHCenter);
  
    statusBar()->addWidget(curIterLabel);
    statusBar()->addWidget(curPluginLabel);
}
void LifeWindow::about() {
    QMessageBox::about(this,
                       tr("About"),
                       tr("<h2>Conway's Game Of Life</h2>"
                          "<p>Conway's Game Of Life using Qt.</p>"
                          "<p>variations of the game are supported via plugins.</p>"
                          "<p>By Jeremiah LaRocco.</p>"));
}

void LifeWindow::loadPlugins() {
    QDir pluginDir(QApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginDir.dirName().toLower() == "debug" ||
        pluginDir.dirName().toLower == "release")
        pluginDir.cdUp();

#elif defined(Q_OS_MAC)
    if (pluginDir.dirName() == "MacOS") {
        pluginDir.cdUp();
        pluginDir.cdUp();
        pluginDir.cdUp();
    }
#endif

    if (!pluginDir.cd("plugins"))
        return;
    foreach(QString fileName, pluginDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginDir.absoluteFilePath(fileName));
        if (LifePlugin *interface = qobject_cast<LifePlugin *>(loader.instance())) {
            interface->readSettings(settings);
            plugins[interface->name()] = interface;
        } else {
            ;
            // qDebug() << fileName << " is not a plugin?!";
        }
    }

    if (plugins.begin()!=plugins.end()) {
        curPlugin = plugins.begin().value()->name();
        life->setPlugin(plugins[curPlugin]);
    }
}

void LifeWindow::pluginChanged() {
    QAction *action = qobject_cast<QAction *>(QObject::sender());
    if (action==0) return;

    curPlugin = action->text();
    life->stop();
    life->setPlugin(plugins[curPlugin]);

    curIterLabel->setText(tr("Current plugin: %1").arg(curPlugin));

}

void LifeWindow::updateIteration(int iter) {
    curIterLabel->setText(tr("Iteration: %1").arg(iter));
}
