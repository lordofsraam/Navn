#include "textfile.h"

TextFile::TextFile(const Flux::string &FileName) : filename(FileName), lasterror(static_cast<FileIOErrors>(NULL))
{
  std::ifstream in(FileName.c_str());
  while(std::getline(in,line.std_str()))
  {
    SingleLineBuffer += line;
    lines.push_back(line);
  }
  Contents = lines;
}

TextFile::TextFile(std::vector<Flux::string> conts) { lines = conts; Contents = conts; }

TextFile::~TextFile() {}

Flux::string TextFile::DecodeLastError()
{
  switch(this->lasterror)
  {
    case FILE_IO_OK:
      return "Success (FILE_IO_OK)";
    case FILE_IO_MEMORY:
      return "Insufficient Memory (FILE_IO_MEMORY)";
    case FILE_IO_PARAMS:
      return "Insufficient Parameters (FILE_IO_PARAMS)";
    case FILE_IO_NOEXIST:
      return "File does not exist (FILE_IO_NOEXIST)";
    case FILE_IO_FAILURE:
      return "I/O Failure (FILE_IO_FAILURE)";
    case FILE_IO_EXISTS:
      return "File Exists (FILE_IO_EXISTS)";
    case FILE_IO_UNKNOWN:
      return "Unknown File Error (FILE_IO_UNKNOWN)";
    case FILE_IO_EXCEPTION:
      return "Exception was caught (FILE_IO_EXCEPTION)";
    default:
      return "Unknown file error.";
  }
  return "Unknown file error.";
}

int TextFile::NumberOfLines() { return lines.size(); }

FileIOErrors TextFile::GetLastError() { return lasterror; }

Flux::string TextFile::GetFilename() { return filename; }

void TextFile::Edit(int ln, Flux::string fs) { lines[ln] = fs; Contents[ln] = fs; }

void TextFile::Clear() { lines.clear(); Contents.clear(); }

bool TextFile::Empty()
{
  if(lines.empty())
    return true;
  for (unsigned i = 0; i < lines.size(); i++) { if (!lines[i].empty()) return false; }
  return true;
}
//This function is REQUIRED by the bot to load modules, !! DO NOT MODIFY !!
FileIOErrors TextFile::Copy(const Flux::string &dest)
{
  if(dest.empty() || this->filename.empty())
    return (this->lasterror = FILE_IO_PARAMS);

  struct stat s;
  if (stat(dest.c_str(), &s) == -1)
    return (this->lasterror = FILE_IO_NOEXIST);
  if(stat(this->filename.c_str(), &s) == -1)
    return (this->lasterror = FILE_IO_NOEXIST);
  else if (!S_ISREG(s.st_mode))
    return (this->lasterror = FILE_IO_NOEXIST);

  std::ifstream source(this->filename.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!source.is_open())
    return (this->lasterror = FILE_IO_NOEXIST);
  std::ofstream target(dest.c_str(), std::ios_base::in | std::ios_base::binary);
  if(!target.is_open()){
    source.close();
    return (this->lasterror = FILE_IO_NOEXIST);
  }

  int want = s.st_size;
  char *buffer = new char[s.st_size];
  while (want > 0 && !source.fail() && !target.fail())
  {
    source.read(buffer, want);
    int read_len = source.gcount();

    target.write(buffer, read_len);
    want -= read_len;
  }
  delete [] buffer;

  source.close();
  target.close();

  return !source.fail() && !target.fail() ? (this->lasterror = FILE_IO_OK) : (this->lasterror = FILE_IO_FAILURE);
}

/**
 * \fn Flux::string TempFile(const Flux::string &file)
 * \brief Creates a temporary file name for use in navn, can be quite useful.
 * \param Flux::string The Flux::string of the file location/name
 * NOTE: THIS _MUST_ HAVE 6 X's (XXXXXX) to work properly.
 */
Flux::string TextFile::TempFile(const Flux::string &file){
  char *tmp_output = strdup(file.c_str());
  int target_fd = mkstemp(tmp_output);
  if (target_fd == -1 || close(target_fd) == -1)
  {
    free(tmp_output);
    return "";
  }
  Flux::string filestring = tmp_output;
  free(tmp_output);
  return filestring;
}

/** Check if a file exists
 * \param filename The file
 * \return true if the file exists, false if it doens't
 */
bool TextFile::IsFile(const Flux::string &filename)
{
  struct stat fileinfo;
  if (!stat(filename.c_str(), &fileinfo))
    return true;
  return false;
}

bool TextFile::WriteToDisk(const Flux::string &FileName)
{
  std::ofstream f(FileName.c_str());
  if (f.is_open())
  {
    if (Contents != lines)
    {
      for (unsigned i = 0; i < Contents.size(); i++)
      {
	f << Contents[i].strip() << "\n";
      }
    }
    else
    {
      for (unsigned i = 0; i < lines.size(); i++)
      {
	f << lines[i].strip() << "\n";
      }
    }
    f.close();
    this->lasterror = FILE_IO_OK;
    return true;
  }
  else { this->lasterror = FILE_IO_NOEXIST; return false; }
}

Flux::string TextFile::SingleLine() { return SingleLineBuffer; }

Flux::string TextFile::Extension()
{
  if (filename.search('.'))
  {
    Flux::string ext = "";
    for (unsigned i = filename.length()-1; filename[i] != '.' && i < filename.size(); i--)
    {
      ext += filename[i];
    }
    return ext;
  }
  else { return ""; }
}