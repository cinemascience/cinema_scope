#include <QString>

class CinDBReader
{
    private:

    public:
        CinDBReader();
        int init();
        int readCSV(QString path);
};

