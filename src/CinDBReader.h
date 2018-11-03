#ifndef CINDBREADER_H
#define CINDBREADER_H

class CinDBReader
{
    private:

    public:
        CinDBReader();
        int init();
        int readCSV(const char *path);
};

#endif // CINDBREADER_H
