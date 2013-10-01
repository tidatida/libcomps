/* libcomps - C alternative to yum.comps library
 * Copyright (C) 2013 Jindrich Luza
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to  Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA
 */

#include "comps_docgroup.h"

void comps_docgroup_create(COMPS_DocGroup* group, COMPS_Object **args) {
    (void)args;
    group->properties = (COMPS_ObjDict*)comps_object_create(
                                                  &COMPS_ObjDict_ObjInfo, NULL);
    group->name_by_lang = (COMPS_ObjDict*)comps_object_create(
                                                  &COMPS_ObjDict_ObjInfo, NULL);
    group->desc_by_lang = (COMPS_ObjDict*)comps_object_create(
                                                  &COMPS_ObjDict_ObjInfo, NULL);
    group->packages = (COMPS_ObjList*)comps_object_create(
                                                  &COMPS_ObjList_ObjInfo, NULL);
}
COMPS_CREATE_u(docgroup, COMPS_DocGroup)

void comps_docgroup_copy(COMPS_DocGroup *group_dst,
                         COMPS_DocGroup *group_src) {
    group_dst->properties = (COMPS_ObjDict*)comps_object_copy(
                                       (COMPS_Object*)group_src->properties);
    group_dst->name_by_lang = (COMPS_ObjDict*)comps_object_copy(
                                    (COMPS_Object*)group_src->name_by_lang);
    group_dst->desc_by_lang = (COMPS_ObjDict*)comps_object_copy(
                                    (COMPS_Object*)group_src->desc_by_lang);
    group_dst->packages = (COMPS_ObjList*)comps_object_copy(
                                    (COMPS_Object*)group_src->packages);
}
COMPS_COPY_u(docgroup, COMPS_DocGroup)    /*comps_utils.h macro*/

static void comps_docgroup_destroy(COMPS_DocGroup *group) {
    
    COMPS_OBJECT_DESTROY(group->properties);
    COMPS_OBJECT_DESTROY(group->name_by_lang);
    COMPS_OBJECT_DESTROY(group->desc_by_lang);
    COMPS_OBJECT_DESTROY(group->packages);
}
COMPS_DESTROY_u(docgroup, COMPS_DocGroup) /*comps_utils.h macro*/

COMPS_STRPROP_SETTER(group, COMPS_DocGroup, id) /*comps_utils.h macro*/
COMPS_STRPROP_SETTER(group, COMPS_DocGroup, name) /*comps_utils.h macro*/
COMPS_STRPROP_SETTER(group, COMPS_DocGroup, desc) /*comps_utils.h macro*/
COMPS_NUMPROP_SETTER(group, COMPS_DocGroup, def) /*comps_utils.h macro*/
COMPS_NUMPROP_SETTER(group, COMPS_DocGroup, uservisible) /*comps_utils.h macro*/
COMPS_NUMPROP_SETTER(group, COMPS_DocGroup, display_order) /*comps_utils.h macro*/
COMPS_STRPROP_SETTER(group, COMPS_DocGroup, langonly) /*comps_utils.h macro*/

COMPS_PROP_GETTER(group, COMPS_DocGroup, id) /*comps_utils.h macro*/
COMPS_PROP_GETTER(group, COMPS_DocGroup, name) /*comps_utils.h macro*/
COMPS_PROP_GETTER(group, COMPS_DocGroup, desc) /*comps_utils.h macro*/
COMPS_PROP_GETTER(group, COMPS_DocGroup, def) /*comps_utils.h macro*/
COMPS_PROP_GETTER(group, COMPS_DocGroup, uservisible) /*comps_utils.h macro*/
COMPS_PROP_GETTER(group, COMPS_DocGroup, display_order) /*comps_utils.h macro*/
COMPS_PROP_GETTER(group, COMPS_DocGroup, langonly) /*comps_utils.h macro*/

COMPS_DOCOBJ_GETOBJLIST(docgroup, COMPS_DocGroup, packages, packages)
COMPS_DOCOBJ_SETOBJLIST(docgroup, COMPS_DocGroup, packages, packages)

void comps_docgroup_add_package(COMPS_DocGroup *group,
                                COMPS_DocGroupPackage *package) {

    if (group == NULL || package == NULL) {
        assert(0);
        return;
    }
    if (group->packages == NULL) {
        group->packages = (COMPS_ObjList*) comps_object_create(
                                                &COMPS_ObjList_ObjInfo, NULL);
    }
    comps_objlist_append_x(group->packages, (COMPS_Object*)package);
}

COMPS_ObjList* comps_docgroup_get_packages(COMPS_DocGroup *group, char *name,
                                       COMPS_PackageType type) {
    COMPS_ObjListIt *it;
    COMPS_ObjList *ret;
    unsigned int matched, matched_max;
    matched_max = 0;
    #define package ((COMPS_DocGroupPackage*)it->comps_obj)
    COMPS_Str *objname = comps_str(name);
    if (!group) return NULL;

    ret = (COMPS_ObjList*)comps_object_create(&COMPS_ObjList_ObjInfo, NULL);

    if (name != NULL) matched_max++;
    if (type != COMPS_PACKAGE_UNKNOWN) matched_max++;

    for (it = (group->packages)?group->packages->first:NULL;
         it != NULL; it = it->next) {
        matched = 0;
        if ((name != NULL) && COMPS_OBJECT_CMP(package->name, objname))
            matched++;
        if ((type != COMPS_PACKAGE_UNKNOWN) && package->type  == type)
            matched++;
        if (matched == matched_max) {
            comps_objlist_append(ret, (COMPS_Object*)package);
        }
    }
    COMPS_OBJECT_DESTROY(objname);
    return ret;
}


signed char comps_docgroup_cmp_u(COMPS_Object *group1, COMPS_Object *group2) {
    #define _group1 ((COMPS_DocGroup*)group1)
    #define _group2 ((COMPS_DocGroup*)group2)

    //printf("group cmp start\n");
    if (!comps_object_cmp((COMPS_Object*)_group1->properties,
                          (COMPS_Object*)_group2->properties)) {
        //printf("Group properties cmp fail\n");
        return 0;
    }
    //printf("group cmp properties pass\n");
    if (!comps_object_cmp((COMPS_Object*)_group1->name_by_lang,
                          (COMPS_Object*)_group2->name_by_lang)) {
        //printf("Group name_by_lang cmp fail\n");
        return 0;
    }
    //printf("group cmp name_by_lang pass\n");
    if (!comps_object_cmp((COMPS_Object*)_group1->desc_by_lang,
                          (COMPS_Object*)_group2->desc_by_lang)) {
        //printf("Group desc_by_lang cmp fail\n");
        return 0;
    }
    //printf("group cmp desc_by_lang pass\n");
    if (!comps_object_cmp((COMPS_Object*)_group1->packages,
                          (COMPS_Object*)_group2->packages)) {
        //printf("Group packages cmp fail\n");
        return 0;
    }
    //printf("group cmp packages pass\n");
    return 1;
    #undef _group1
    #undef _group2
}

char __comps_docgroup_idcmp(void *g1, void *g2) {
    COMPS_Object *obj1, *obj2;
    char ret;
    obj1 = comps_objdict_get(((COMPS_DocGroup*)g1)->properties, "id");
    obj2 = comps_objdict_get(((COMPS_DocGroup*)g2)->properties, "id");
    COMPS_OBJECT_DESTROY(obj1);
    COMPS_OBJECT_DESTROY(obj2);
    ret = comps_object_cmp(obj1, obj2);
    return ret;
}

COMPS_DocGroup* comps_docgroup_union(COMPS_DocGroup *g1, COMPS_DocGroup *g2) {
    COMPS_DocGroup *res;
    COMPS_ObjListIt *it;
    COMPS_HSListItem *hsit;
    COMPS_Set *set;
    COMPS_DocGroupPackage *newpkg;
    COMPS_HSList *pairs;

    res = (COMPS_DocGroup*)comps_object_create(&COMPS_DocGroup_ObjInfo, NULL);
    COMPS_OBJECT_DESTROY(res->properties);

    res->properties = comps_objdict_clone(g2->properties);
    pairs = comps_objdict_pairs(g1->properties);
    for (hsit = pairs->first; hsit != NULL; hsit = hsit->next) {
        comps_objdict_set(res->properties, ((COMPS_RTreePair*)hsit->data)->key,
                          ((COMPS_RTreePair*)hsit->data)->data);
    }
    comps_hslist_destroy(&pairs);

    set = comps_set_create();
    comps_set_init(set, NULL, NULL, NULL, &comps_docpackage_cmp_set);
    it = g1->packages?g1->packages->first:NULL;
    for (; it != NULL; it = it->next) {
        comps_set_add(set, (void*)it->comps_obj);
    }
    it = g2->packages?g2->packages->first:NULL;
    for (; it != NULL; it = it->next) {
        comps_set_add(set, (void*)it->comps_obj);
    }
    //res->packages = (COMPS_ObjList*)comps_object_create(&COMPS_ObjList_ObjInfo, NULL);
    //comps_list_init(res->packages);
    for (hsit = set->data->first; hsit!= NULL; hsit = hsit->next) {
        newpkg = (COMPS_DocGroupPackage*)comps_object_copy(
                                            (COMPS_Object*)hsit->data);
        comps_docgroup_add_package(res, newpkg);
    }
    comps_set_destroy(&set);
    comps_object_destroy((COMPS_Object*)res->name_by_lang);
    comps_object_destroy((COMPS_Object*)res->desc_by_lang);
    res->name_by_lang = comps_objdict_union(g1->name_by_lang, g2->name_by_lang);
    res->desc_by_lang = comps_objdict_union(g1->desc_by_lang, g2->desc_by_lang);
    return res;
}

COMPS_DocGroup* comps_docgroup_intersect(COMPS_DocGroup *g1,
                                         COMPS_DocGroup *g2) {
    COMPS_DocGroup *res;
    COMPS_ObjListIt *it;
    COMPS_HSListItem *hsit;
    COMPS_Set *set;
    COMPS_DocGroupPackage *newpkg;
    COMPS_HSList *pairs1, *pairs2;

    res = (COMPS_DocGroup*)comps_object_create(&COMPS_DocGroup_ObjInfo, NULL);
    set = comps_set_create();
    //comps_objrtree_paircmp(void *obj1, void *obj2) {
    comps_set_init(set, NULL, NULL, NULL, &comps_objrtree_paircmp);

    pairs1 = comps_objdict_pairs(g1->properties);
    for (hsit = pairs1->first; hsit != NULL; hsit = hsit->next) {
        comps_set_add(set, hsit->data);
    }
    pairs2 = comps_objdict_pairs(g2->properties);
    for (hsit = pairs2->first; hsit != NULL; hsit = hsit->next) {
        if (comps_set_in(set, hsit->data)) {
            comps_objdict_set(res->properties,
                              ((COMPS_RTreePair*)hsit->data)->key,
                              ((COMPS_RTreePair*)hsit->data)->data);
        }
    }
    comps_hslist_destroy(&pairs1);
    comps_hslist_destroy(&pairs2);
    comps_set_clear(set);
    //res->packages = (COMPS_ObjList*)comps_object_create(&COMPS_ObjList_ObjInfo,
    //                                                    NULL);

    //set = comps_set_create();
    comps_set_init(set, NULL, NULL, NULL, &comps_docpackage_cmp_set);

    for (it = g1->packages->first; it != NULL; it = it->next) {
        comps_set_add(set, it->comps_obj);
    }
    for (it = g2->packages->first; it != NULL; it = it->next) {
        if (comps_set_in(set, it->comps_obj)) {
            newpkg = (COMPS_DocGroupPackage*)
                     comps_object_copy(it->comps_obj);
            comps_docgroup_add_package(res, newpkg);
        }
    }
    comps_set_destroy(&set);
    return res;
}



void comps_docgroup_xml(COMPS_DocGroup *group, xmlTextWriterPtr writer,
                        COMPS_Logger *log) {
    COMPS_ObjListIt *it;
    COMPS_Object *obj;
    COMPS_HSList *pairlist;
    COMPS_HSListItem *hsit;

    static char* props[] = {"id", "name", "name", "desc",
                            "desc", "def", "uservisible", "display_order",
                            "langonly"};
    static size_t type[] =   {0, 0, offsetof(COMPS_DocGroup, name_by_lang),
                              0, offsetof(COMPS_DocGroup, desc_by_lang), 0,
                              0, 0, 0};
    static char* aliases[] = {NULL, NULL, NULL, "description", "description",
                              "default", NULL, NULL, NULL};
    static char*(*tostrf[])(COMPS_Object*) = {&comps_object_tostr,
                                               &comps_object_tostr,
                                               &comps_object_tostr,
                                               &comps_object_tostr,
                                               &comps_object_tostr,
                                               &__comps_num2boolstr,
                                               &__comps_num2boolstr,
                                               &comps_object_tostr,
                                               &comps_object_tostr};
    char *str;

    xmlTextWriterStartElement(writer, BAD_CAST "group");
    for (int i=0; i<9; i++) {
        //printf("%s\n", props[i]);
        if (!type[i]) {
            obj = comps_objdict_get_x(group->properties, props[i]);
            if (obj) {
                str = tostrf[i](obj);
                __comps_xml_prop((aliases[i])?aliases[i]:props[i], str, writer);
                free(str);
            } else {
                //printf("missing %s\n", props[i]);
            }
        } else {
            pairlist = comps_objdict_pairs(*(COMPS_ObjDict**)
                                           (((char*)group)+type[i]));
            for (hsit = pairlist->first; hsit != NULL; hsit = hsit->next) {
                xmlTextWriterStartElement(writer,
                            (const xmlChar*)((aliases[i])?aliases[i]:props[i]));
                xmlTextWriterWriteAttribute(writer, (xmlChar*) BAD_CAST "xml:lang",
                        (xmlChar*) ((COMPS_ObjRTreePair*)hsit->data)->key);
                str = tostrf[i](((COMPS_ObjRTreePair*)hsit->data)->data);
                xmlTextWriterWriteString(writer, (xmlChar*)str);
                free(str);
                xmlTextWriterEndElement(writer);
            }
            comps_hslist_destroy(&pairlist);
        }
    }
    xmlTextWriterStartElement(writer, (xmlChar*)"packagelist");
    for (it = group->packages->first; it != NULL; it = it->next) {
        comps_docpackage_xml((COMPS_DocGroupPackage*)it->comps_obj, writer, log);
    }
    xmlTextWriterEndElement(writer);
    xmlTextWriterEndElement(writer);
}

COMPS_ObjectInfo COMPS_DocGroup_ObjInfo = {
    .obj_size = sizeof(COMPS_DocGroup),
    .constructor = &comps_docgroup_create_u,
    .destructor = &comps_docgroup_destroy_u,
    .copy = &comps_docgroup_copy_u,
    .obj_cmp = &comps_docgroup_cmp_u
};
