#include <gtest/gtest.h>

#include "../Model/model.h"

TEST(VIEWER, loader_1) {  // нет файла
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  try {
    loader.loadObjModel("../objects/none.obj", model);
  } catch (std::logic_error &e) {
    EXPECT_STREQ("Can't open file", e.what());
  }
}

TEST(VIEWER, loader_2) {  // неправильный индекс вершины
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  try {
    loader.loadObjModel("../objects/ball.obj", model);
  } catch (std::logic_error &e) {
    EXPECT_STREQ("Wrong vertex index", e.what());
  }
}

TEST(VIEWER, loader_3) {  // неправильный индекс грани
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  try {
    loader.loadObjModel("../objects/pyramid.obj", model);
  } catch (std::logic_error &e) {
    EXPECT_STREQ("Wrong facet", e.what());
  }
}

TEST(VIEWER, loader_4) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  try {
    loader.loadObjModel("../objects/diamond.obj", model);
  } catch (std::logic_error &e) {
    EXPECT_STREQ("Wrong vertex", e.what());
  }
}

TEST(VIEWER, loader_5) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  loader.loadObjModel("../objects/t.obj", model);
  EXPECT_EQ((unsigned)48, model->get_amount_vertices());
  EXPECT_EQ((unsigned)48 * 3 + 11 * 4, model->get_amount_facets() / 2);
}

TEST(VIEWER, Affine_Move1) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  s21::Transform transform(model);
  loader.loadObjModel("../objects/t.obj", model);
  transform.Move(2, s21::axes::X);
  float *vertices = model->get_vertices();
  EXPECT_EQ(vertices[0], 2);
  EXPECT_EQ(vertices[3], 2);
  EXPECT_EQ(vertices[6], 2);
  EXPECT_EQ(vertices[9], 2);
  EXPECT_EQ(vertices[12], 0);
}

TEST(VIEWER, Affine_Move2) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  s21::Transform transform(model);
  loader.loadObjModel("../objects/t.obj", model);
  transform.Move(2, s21::axes::Y);
  float *vertices = model->get_vertices();
  EXPECT_EQ(vertices[1], 12);
  EXPECT_EQ(vertices[4], 12);
  EXPECT_EQ(vertices[7], 10);
  EXPECT_EQ(vertices[10], 10);
  EXPECT_EQ(vertices[13], 10);
}

TEST(VIEWER, Affine_Move3) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  s21::Transform transform(model);
  loader.loadObjModel("../objects/t.obj", model);
  transform.Move(2, s21::axes::Z);
  float *vertices = model->get_vertices();
  EXPECT_EQ(vertices[2], 2);
  EXPECT_EQ(vertices[5], 4);
  EXPECT_EQ(vertices[8], 2);
  EXPECT_EQ(vertices[11], 4);
  EXPECT_EQ(vertices[14], 2);
}

TEST(VIEWER, Affine_Move4) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  s21::Transform transform(model);
  loader.loadObjModel("../objects/t.obj", model);
  transform.Move(0.01, s21::axes::X);
  float *vertices = model->get_vertices();
  EXPECT_FLOAT_EQ(vertices[0], 0.01);
  EXPECT_FLOAT_EQ(vertices[3], 0.01);
  EXPECT_FLOAT_EQ(vertices[6], 0.01);
  EXPECT_FLOAT_EQ(vertices[9], 0.01);
  EXPECT_FLOAT_EQ(vertices[12], -1.99);
}

TEST(VIEWER, Affine_Rotate) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  s21::Transform transform(model);
  loader.loadObjModel("../objects/t.obj", model);
  transform.Rotate(2, s21::axes::X);
  float *vertices = model->get_vertices();
  EXPECT_FLOAT_EQ(vertices[1], 9.9939079);
  EXPECT_FLOAT_EQ(vertices[4], 9.9241085);
  EXPECT_FLOAT_EQ(vertices[7], 7.9951267);
  EXPECT_FLOAT_EQ(vertices[10], 7.9253278);
  EXPECT_FLOAT_EQ(vertices[13], 7.9951267);

  EXPECT_FLOAT_EQ(vertices[2], 0.34899497);
  EXPECT_FLOAT_EQ(vertices[5], 2.3477767);
  EXPECT_FLOAT_EQ(vertices[8], 0.27919596);
  EXPECT_FLOAT_EQ(vertices[11], 2.2779777);
  EXPECT_FLOAT_EQ(vertices[14], 0.27919596);
}

TEST(VIEWER, Affine_Scale) {
  s21::Model* model = s21::Model::GetInstance();
  s21::Loader loader;
  s21::Transform transform(model);
  loader.loadObjModel("../objects/t.obj", model);
  transform.Scale(1, 1.5);
  float *vertices = model->get_vertices();

  EXPECT_FLOAT_EQ(vertices[0], 0);
  EXPECT_FLOAT_EQ(vertices[3], 0);
  EXPECT_FLOAT_EQ(vertices[6], 0);
  EXPECT_FLOAT_EQ(vertices[9], 0);
  EXPECT_FLOAT_EQ(vertices[12], -3);

  EXPECT_FLOAT_EQ(vertices[1], 15);
  EXPECT_FLOAT_EQ(vertices[4], 15);
  EXPECT_FLOAT_EQ(vertices[7], 12);
  EXPECT_FLOAT_EQ(vertices[10], 12);
  EXPECT_FLOAT_EQ(vertices[13], 12);

  EXPECT_FLOAT_EQ(vertices[2], 0);
  EXPECT_FLOAT_EQ(vertices[5], 3);
  EXPECT_FLOAT_EQ(vertices[8], 0);
  EXPECT_FLOAT_EQ(vertices[11], 3);
  EXPECT_FLOAT_EQ(vertices[14], 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
