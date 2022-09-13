package db

import (
	"fmt"
	"trackr/src/models"

	"gorm.io/gorm"
)

type ProjectServiceDB struct {
	database *gorm.DB
}

func (service *ProjectServiceDB) GetProjectsByUser(user models.User) ([]models.Project, error) {
	var projects []models.Project
	if result := service.database.Find(&projects, "user_id = ?", user.ID); result.Error != nil {
		return nil, result.Error
	}

	return projects, nil
}

func (service *ProjectServiceDB) GetProjectByIdAndUser(id uint, user models.User) (*models.Project, error) {
	var project models.Project
	if result := service.database.First(&project, "id = ? AND user_id = ?", id, user.ID); result.Error != nil {
		return nil, result.Error
	}

	return &project, nil
}

func (service *ProjectServiceDB) AddProject(project models.Project) error {
	if result := service.database.Create(&project); result.Error != nil {
		return result.Error
	}

	return nil
}

func (service *ProjectServiceDB) UpdateProject(project models.Project) error {
	if result := service.database.Save(&project); result.Error != nil {
		return result.Error
	}

	return nil
}

func (service *ProjectServiceDB) DeleteProjectByIdAndUser(id uint, user models.User) error {
	result := service.database.Delete(&models.Project{}, "id = ? AND user_id = ?", id, user.ID)
	if result.Error != nil {
		return result.Error
	}

	if result.RowsAffected == 0 {
		return fmt.Errorf("can't find a corresponding project")
	}

	return nil
}
