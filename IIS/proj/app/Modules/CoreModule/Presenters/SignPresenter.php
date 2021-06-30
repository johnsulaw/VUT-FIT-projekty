<?php

namespace App\Modules\CoreModule\Presenters;

use Contributte\FormsBootstrap\BootstrapForm;
use Nette;
use Nette\Application\UI\Form;
use Nette\Forms\Controls\BaseControl;


class SignPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
    private $authenticator;
    
    public function __construct(\MyAuthenticator $authenticator, Nette\Database\Context $database) {
        parent::__construct();
        $this->authenticator = $authenticator;
    }
    
    protected function createComponentSignInForm(): Form
    {
        $form = new BootstrapForm;
        $form->addText('username')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Email')
            ->setRequired('Please enter you e-mail.');

        $form->addPassword('password')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Password')
            ->setRequired('Please enter your password.');

        $form->addSubmit('send', 'Sign in');

        $form->onSuccess[] = [$this, 'signInFormSucceeded'];
        return $form;
    }
    
    protected function createComponentSignUpForm() : Form
    {
        $form = new BootstrapForm;

        $form->addText('firstname')
            ->setRequired('Please enter your first name.')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','First name');
            
        $form->addText('lastname')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Last name')   
            ->setRequired('Please enter your last name.');
        
        $form->addText('email')
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','Email')
                ->setRequired()
                ->addRule($form::EMAIL)
                ->addRule(function (BaseControl $input) { // User already exists
                    $mail = $input->getValue();
                    if ($this->database->table('user')->where('guest_mail', $mail)->fetch()){
                        return false;
                    }
                    return true;
                }, 'There is already account registered on this mail');     
        
        $form->addDate('birthdate', 'Date of birth')
            ->setHtmlAttribute('class','form-control')
            ->setRequired('Please enter your birth day.');

        $form->addText('city')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','City')
            ->setRequired('Please enter the city in which you live.');

        $form->addPassword('password')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Password')
            ->setRequired('Please choose a password.');
            
        
        $form->addText('street')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Street')
            ->setRequired('Please enter your address.');
        
        $form->addSubmit('send', 'Sign Up');
        
        $form->onSuccess[] = [$this, 'signUpFormSucceeded'];
        return $form;
    }

    public function signInFormSucceeded(Form $form, \stdClass $values): void
    {
        try {
            $this->getUser()->login($values->username, $values->password);
            $this->getUser()->setExpiration('30 minutes');
            $this->redirect('Homepage:');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError('Wrong email or password');
        }
    }
    
    public function signUpFormSucceeded(Form $form, \stdClass $values) : void
    {
        try{
            $this->authenticator->addUser($values);
            $this->redirect('Homepage:');
        } catch (Exception $ex) {
            $form->addError($ex);
        }
    }

    public function actionOut(): void
    {
        $this->getUser()->logout();
        $this->redirect('Homepage:');
    }


}

