#include "CinImageViewer.h"

#include <string>
#include <iostream>

using namespace std;

bool MyImageView::mLoadImage(QString path, QPixmap *image )
{
    return image->load(path);
}

string MyImageView::constructQueryString(QStringList currentParamnames)
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

void MyImageView :: mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"MYIMAGE: on left mouse press"<<endl;
        currentXloc = e->pos().rx();
        currentYloc = e->pos().ry();
    }
}

void MyImageView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"MYIMAGE: n left mouse release"<<endl;
        lastXloc = e->pos().rx();
        lastYloc = e->pos().ry();
    }
}


void MyImageView::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    //cout<<"MYIMAGE: moving mouse loc: "<< p.rx()<<" "<<p.ry()<<endl;

    string path = "../data/volume-render.cdb"; //change later
    string imagePath;
    QPixmap image;

    int numSliders = paramSet->getNumParameters();
    QStringList paramNames = paramSet->getParameterNames();

    vector<float> currentParamVals;
    for(int i=0;i<paramNames.size();i++)
    {
        float val;
        paramSet->getValue(paramNames[i],&val);
        currentParamVals.push_back(val);
    }

    //get the current values of phi and theta
    /*for(int i=0;i<paramNames.size();i++)
    {
        if(paramNames[i]=="phi")
            paramSet->getValue(paramNames[i],&currentPhi);

        if(paramNames[i]=="theta")
            paramSet->getValue(paramNames[i],&currentTheta);
    }*/

    QSqlQuery qry;
    string queryText;

    if(fabs(p.rx() - currentXloc) > slidePixel && fabs(p.ry() - currentYloc) < slidePixel) //only phi
    {
        if((p.rx() - currentXloc) > 0 && lastXloc < p.rx()) //slide right
        {
            queryText = "SELECT MIN(phi) FROM " + mTableName.toStdString() + " WHERE phi > :phi";
            qry.prepare(QString::fromStdString(queryText));
            qry.bindValue(":phi", currentPhi);
            qry.exec();
            while (qry.next())
            {
                currentPhi = qry.value(0).toFloat();
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
                //cout<<qry.value(numSliders).toString().toStdString()<<endl;
                imagePath = path + "/" + qry.value(numSliders).toString().toStdString();
            }

            //Modify the slider value at the end as a result of drag
        }

        else if((p.rx() - currentXloc) < 0  && lastXloc > p.rx()) //slide left
        {


            queryText = "SELECT MAX(phi) FROM " + mTableName.toStdString() + " WHERE phi < :phi";
            qry.prepare(QString::fromStdString(queryText));
            qry.bindValue(":phi", currentPhi);
            qry.exec();
            while (qry.next())
            {
                currentPhi = qry.value(0).toFloat();
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

            //If current phi is equal to minimum phi then set current phi to maximum phi
            //this may not be needed if we use sliders properly and do not need circular movement
            //hardcoded value for now
            if(currentPhi==0)
                currentPhi=165;

            //Modify the slider value at the end as a result of drag
        }
    }
    else if(fabs(p.ry() - currentYloc) > slidePixel && fabs(p.rx() - currentXloc) < slidePixel) //only theta
    {
        if((p.ry() - currentYloc) < 0) //slide up
        {
            cout<<"up"<<" "<<p.ry()<<" "<<currentYloc<<endl;
        }
        else if((p.ry() - currentYloc) > 0) //slide down
        {
            cout<<"down"<<" "<<p.ry()<<" "<<currentYloc<<endl;
        }
    }
    else if(fabs(p.rx() - currentXloc) > slidePixel && fabs(p.ry() - currentYloc) > slidePixel) //both theta and phi
    {
        cout<<"both"<<endl;
    }

    //Load the image finally
    if(!mLoadImage(QString::fromStdString(imagePath),&image))
    {
        cout<<"image loading failed!!"<<endl;
    }
    else
    {
        this->sceneObj->addPixmap(image);
    }

    lastXloc = p.rx();
    lastYloc = p.ry();
}



