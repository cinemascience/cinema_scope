#include "CinImageViewer.h"

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

    float min,max;
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

    if(fabs(p.rx() - currentXloc) > slidePixelHor && fabs(p.ry() - currentYloc) < slidePixelHor) //only phi
    {
        if((p.rx() - currentXloc) > 0 && lastXloc < p.rx()) //slide right
        {
            //qDebug()<<"slide right";

            float next;
            bool nbool = paramSet->getNextValue( "phi", currentPhi, next );

            if(!nbool)
            {
                paramSet->getMinMax("phi", min, max);
                //qDebug()<<"get min max: "<<min<<" "<<max<<endl;
                next=min;
            }

            //qDebug() << "PHI NEXT QUERY (" << nbool << ") : " << currentPhi << ": " << next;

            // Modify the slider value at the end as a result of drag
            paramSet->changeParameter("phi", next);
        }

        else if((p.rx() - currentXloc) < 0 && lastXloc > p.rx()) //slide left
        {
            //qDebug()<<"slide left";

            float prev;
            bool nbool = paramSet->getPrevValue( "phi", currentPhi, prev );
            if(!nbool)
            {
                paramSet->getMinMax("phi", min, max);
                //qDebug()<<"get min max: "<<min<<" "<<max<<endl;
                prev=max;
            }

            //qDebug() << "PHI PREV QUERY (" << nbool << ") : " << currentPhi << ": " << prev;

            //Modify the slider value at the end as a result of drag
            paramSet->changeParameter("phi", prev);
        }
    }
    else if(fabs(p.ry() - currentYloc) > slidePixelVer && fabs(p.rx() - currentXloc) < slidePixelVer) //only theta
    {
        if((p.ry() - currentYloc) < 0 && lastYloc > p.ry()) //slide up
        {
            float next;
            bool nbool = paramSet->getNextValue( "theta", currentTheta, next );

            if(!nbool)
            {
                paramSet->getMinMax("theta", min, max);
                //qDebug()<<"get min max: "<<min<<" "<<max<<endl;
                next=min;
            }

            //qDebug() << "THETA NEXT QUERY (" << nbool << ") : " << currentTheta << ": " << next;

            // Modify the slider value at the end as a result of drag
            paramSet->changeParameter("theta", next);
        }

        else if((p.ry() - currentYloc) > 0 && lastYloc < p.ry()) //slide down
        {
            float prev;
            bool nbool = paramSet->getPrevValue( "theta", currentTheta, prev );
            if(!nbool)
            {
                paramSet->getMinMax("theta", min, max);
                //qDebug()<<"get min max: "<<min<<" "<<max<<endl;
                prev=max;
            }

            //qDebug() << "THETA PREV QUERY (" << nbool << ") : " << currentTheta << ": " << prev;

            // Modify the slider value at the end as a result of drag
            paramSet->changeParameter("theta", prev);
        }
    }

    lastXloc = p.rx();
    lastYloc = p.ry();
}

