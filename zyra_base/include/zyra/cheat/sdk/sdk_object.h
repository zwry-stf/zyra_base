#pragma once
#include <zyra/util/class.h>


#define _sdk_class(_class) \
zyra_remove_default_constructor(_class) \
zyra_remove_copy_constructor(_class) \
zyra_remove_move_constructor(_class)