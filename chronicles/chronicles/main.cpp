
#include "app.h"

static void _parseArgs(int argc, char** argv, AppConfig &config) {
   auto begin = argv + 1;
   auto end = argv + argc;

   for (auto arg = begin; arg < end; ++arg) {
      if (!strcmp(*arg, "-assets") && ++arg < end) {
         config.assetFolder = *arg;
      }
   }
}

int main(int argc, char** argv)
{
   AppConfig config;
   _parseArgs(argc, argv, config);

   auto app = appCreate(config);

   appCreateWindow(app, WindowConfig{ 1280, 720, "CRN4.EXE" });

   while (appRunning(app)) {      
      appStep(app);
   }

   appDestroy(app);
   return 0;
}
