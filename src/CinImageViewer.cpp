#include "CinImageViewer.h"
#include <QPixmapCache>
#include <QFileInfo>
#include <string>
#include <iostream>

using namespace std;

bool CinImageView::loadImage(const QString &path)
{
    QPixmap pixmap;
    QPixmap *found = NULL;
    bool result = true;

    // is it in the cache?
    if (mCache.find(path, found)) {
        // yes, just put it in the scene
        pixmap.load(path);
        mCache.insert(path, pixmap);
        sceneObj->addPixmap(pixmap);
        result = true;
        // debug
        QFileInfo imFile(path);
        qDebug() << "CINIMAGEVIEW found: " << imFile.fileName(); 
    } else {
        // no, create a new one 
        found = new QPixmap();
        found->load(path);
        mCache.insert(path, *found);
        sceneObj->addPixmap(*found);
        result = true;
        // debug
        QFileInfo imFile(path);
        qDebug() << "CINIMAGEVIEW load : " << imFile.fileName(); 
    }

    return result; 
}

string CinImageView::constructQueryString(QStringList currentParamnames)
{
    string query;

    query = "SELECT * FROM " + mTableName.toStdString() + " WHERE ";
    for(int i=0;i<currentParamnames.size();i++)
    {
        if(i<currentParamnames.size()-1)
            query = query + currentParamnames[i].toStdString() + "=:" + currentParamnames[i].toStdString() + " AND ";
        else
            query = query + currentParamnames[i].toStdString() + "=:" + currentParamnames[i].toStdString();
    }
    return query;
}

// key is ignored for now, since we're only loading one artifact 
void CinImageView::onLoadImage(const QString &key, const QString &path)
{
    loadImage(path);
}

void CinImageView::wheelEvent(QWheelEvent * event)
{
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);

    } else {
        // Zooming out
         scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void CinImageView :: mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //cout<<"CINIMAGE: on left mouse press"<<endl;
        currentXloc = e->pos().rx();
        currentYloc = e->pos().ry();
    }
}

void CinImageView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //cout<<"CINIMAGE: n left mouse release"<<endl;
        lastXloc = e->pos().rx();
        lastYloc = e->pos().ry();
    }
}

void CinImageView::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();

    string path = dbPath.toStdString();
    string imagePath;

    int numSliders = paramSet->getNumParameters();
    QStringList paramNames = paramSet->getParameterNames();

    vector<float> currentParamVals;
    for(int i=0;i<paramNames.size();i++)
    {
        float val;
        paramSet->getValue(paramNames[i], val);
        currentParamVals.push_back(val);
    }

    //get the current values of phi and theta
    for(int i=0;i<paramNames.size();i++)
    {
        if(paramNames[i]=="phi")
            paramSet->getValue(paramNames[i], currentPhi);

        if(paramNames[i]=="theta")
            paramSet->getValue(paramNames[i], currentTheta);
    }

    QSqlQuery qry;
    string queryText;
    float min,max;

    if(fabs(p.rx() - currentXloc) > slidePixel && fabs(p.ry() - currentYloc) < slidePixel) //only phi
    {
        if((p.rx() - currentXloc) > 0 && lastXloc < p.rx()) //slide right
        {
            // queryText = QString("SELECT MIN(phi) FROM %1 WHERE phi > :phi").arg(mTableName);
            // qry.prepare(QString::fromStdString(queryText));
            qry.prepare(QString("SELECT MIN(phi) FROM %1 WHERE phi > :phi").arg(mTableName));
            qry.bindValue(":phi", currentPhi);
            qry.exec();
                // begin testing
            // float next;
            // bool nbool = paramSet->getNextValue( "phi", currentPhi, next );
            // float prev;
            // bool pbool = paramSet->getPrevValue( "phi", currentPhi, prev );
            // qDebug() << "PHI PREV QUERY (" << pbool << ") : " << currentPhi << ": " << prev;
            // qDebug() << "PHI NEXT QUERY (" << nbool << ") : " << currentPhi << ": " << next;
                // end testing
            while (qry.next())
            {
                currentPhi = qry.value(0).toFloat();
            }
                // begin testing
            // qDebug() << "PHI NEXT QUERY: " << currentPhi;
            // qDebug() << " ";
                // end testing

            //now get the correct image
            queryText = constructQueryString(paramNames);
            qry.prepare(QString::fromStdString(queryText));
            for(int i=0;i<numSliders;i++)
            {
                if(paramNames[i].toStdString()=="phi")
                    qry.bindValue(":phi", currentPhi);
                else
                {
                    string s;
                    s = ":"+paramNames[i].toStdString();
                    qry.bindValue(QString::fromStdString(s), currentParamVals[i]);
                }
            }
            qry.exec();
            while (qry.next())
            {
                //cout<<qry.value(numSliders).toString().toStdString()<<endl;
                imagePath = path + "/" + qry.value(numSliders).toString().toStdString();
            }

            // Modify the slider value at the end as a result of drag
            paramSet->changeParameter("phi", currentPhi);
        }

        else if((p.rx() - currentXloc) < 0  && lastXloc > p.rx()) //slide left
        {
            //If current phi is equal to minimum phi then set current phi to maximum phi
            paramSet->getMinMax("phi", min, max);
            if( (currentPhi-min) < 0.0000001)
                currentPhi=max;
            else
            {
                queryText = "SELECT MAX(phi) FROM " + mTableName.toStdString() + " WHERE phi < :phi";
                qry.prepare(QString::fromStdString(queryText));
                qry.bindValue(":phi", currentPhi);
                qry.exec();
                while (qry.next())
                {
                    currentPhi = qry.value(0).toFloat();
                }
            }

            //now get the correct image
            queryText = constructQueryString(paramNames);
            qry.prepare(QString::fromStdString(queryText));
            for(int i=0;i<numSliders;i++)
            {
                if(paramNames[i].toStdString()=="phi")
                    qry.bindValue(":phi", currentPhi);
                else
                {
                    string s;
                    s = ":"+paramNames[i].toStdString();
                    qry.bindValue(QString::fromStdString(s), currentParamVals[i]);
                }
            }
            qry.exec();
            while (qry.next())
            {
                cout<<qry.value(numSliders).toString().toStdString()<<endl;
                imagePath = path + "/" + qry.value(numSliders).toString().toStdString();
            }

            //Modify the slider value at the end as a result of drag
            paramSet->changeParameter("phi", currentPhi);
        }
    }
    else if(fabs(p.ry() - currentYloc) > slidePixel && fabs(p.rx() - currentXloc) < slidePixel) //only theta
    {
        if((p.ry() - currentYloc) < 0 && lastYloc > p.ry()) //slide up
        {
            queryText = "SELECT MIN(theta) FROM " + mTableName.toStdString() + " WHERE theta > :theta";
            qry.prepare(QString::fromStdString(queryText));
            qry.bindValue(":theta", currentTheta);
            qry.exec();
            while (qry.next())
            {
                currentTheta = qry.value(0).toFloat();
            }

            //now get the correct image
            queryText = constructQueryString(paramNames);
            qry.prepare(QString::fromStdString(queryText));
            for(int i=0;i<numSliders;i++)
            {
                if(paramNames[i].toStdString()=="theta")
                    qry.bindValue(":theta", currentTheta);
                else
                {
                    string s;
                    s = ":"+paramNames[i].toStdString();
                    qry.bindValue(QString::fromStdString(s), currentParamVals[i]);
                }
            }
            qry.exec();
            while (qry.next())
            {
                //cout<<qry.value(numSliders).toString().toStdString()<<endl;
                imagePath = path + "/" + qry.value(numSliders).toString().toStdString();
            }

            // Modify the slider value at the end as a result of drag
            paramSet->changeParameter("theta", currentTheta);
        }

        else if((p.ry() - currentYloc) > 0 && lastYloc < p.ry()) //slide down
        {
            //If current theta is equal to minimum theta then set current theta to maximum theta
            paramSet->getMinMax("theta", min, max);
            if( (currentTheta-min) < 0.0000001)
                currentTheta=max;
            else
            {
                queryText = "SELECT MAX(theta) FROM " + mTableName.toStdString() + " WHERE theta < :theta";
                qry.prepare(QString::fromStdString(queryText));
                qry.bindValue(":theta", currentTheta);
                qry.exec();

                while (qry.next())
                {
                    currentTheta = qry.value(0).toFloat();
                }
            }

            //now get the correct image
            queryText = constructQueryString(paramNames);
            qry.prepare(QString::fromStdString(queryText));
            for(int i=0;i<numSliders;i++)
            {
                if(paramNames[i].toStdString()=="theta")
                    qry.bindValue(":theta", currentTheta);
                else
                {
                    string s;
                    s = ":"+paramNames[i].toStdString();
                    qry.bindValue(QString::fromStdString(s), currentParamVals[i]);
                }
            }
            qry.exec();
            while (qry.next())
            {
                imagePath = path + "/" + qry.value(numSliders).toString().toStdString();
            }

            // Modify the slider value at the end as a result of drag
            paramSet->changeParameter("theta", currentTheta);
        }
    }
    else if(fabs(p.rx() - currentXloc) > slidePixel && fabs(p.ry() - currentYloc) > slidePixel) //both theta and phi
    {
        cout<<"both"<<endl;
    }

    // Load the image
    loadImage(QString::fromStdString(imagePath));

    lastXloc = p.rx();
    lastYloc = p.ry();
}

