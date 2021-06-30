<?php

namespace App\Modules\CoreModule\Presenters;

use Contributte\FormsBootstrap\BootstrapForm;
use Nette;
use Nette\Application\UI\Form;

final class ProfilePresenter extends \Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
    
    public function renderDefault() : void
    {
       $user = $this->template->user->getId();
       
       $guest_row = $this->database->table('guest')->where('mail_address', $user)->fetch();
       $user_row = $this->database->table('user')->where('guest_mail', $user)->fetch();

       list($date, $time) = explode(" ", $guest_row->birth_date);

       $this->template->firstname = $guest_row->first_name;
       $this->template->lastname = $guest_row->last_name;
       $this->template->birthdate = $date;
       $this->template->city = $guest_row->city;
       $this->template->street = $guest_row->street;
    }
   
    protected function createComponentProfileEditForm(): Form
    {
        $user = $this->template->user->getId();
       
        $form = new BootstrapForm;
        
        $guest_row = $this->database->table('guest')->where('mail_address', $user)->fetch();

        $form->addText('firstname', 'First name:')
            ->setDefaultValue($guest_row->first_name)
            ->setRequired('Please enter your first name.');
        $form->addText('lastname', 'Last name:')
            ->setDefaultValue($guest_row->last_name)
            ->setRequired('Please enter your last name.');
        $form->addDate('birthdate', 'Birthday:')->setRequired('Please enter your birthday.');
        $form->addText('city', 'City:')
            ->setDefaultValue($guest_row->city)->setRequired('Please enter your city.');
        $form->addText('street', 'Address:')
            ->setDefaultValue($guest_row->street)->setRequired('Please enter your address.');
        $form->addSubmit('send', 'Confirm changes');

        $form->onSuccess[] = [$this, 'profileEditFormSucceeded'];
        
        return $form;
    }
    
    public function profileEditFormSucceeded(Form $form, \stdClass $values): void
    {
        $user = $this->template->user->getId();
        
        $this->database->query('UPDATE guest SET', [
            'first_name' => $values->firstname,
            'last_name' => $values->lastname,
            'birth_date' => $values->birthdate,
            'city' => $values->city,
            'street' => $values->street,
            ], 'WHERE mail_address = ?', $this->user->id);
    
        $this->redirect('Profile:');
    }
}