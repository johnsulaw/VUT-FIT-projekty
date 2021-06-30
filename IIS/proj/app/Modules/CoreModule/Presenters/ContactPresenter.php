<?php

namespace App\Modules\CoreModule\Presenters;
use Nette;
use Contributte\FormsBootstrap\BootstrapForm;
use Nette\Mail\Message;

final class ContactPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;

    protected function createComponentContactForm()
	{
		$form = new BootstrapForm;
        $form->addText('name')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','First name')
			->addRule($form::FILLED, 'Please enter your name');
        $form->addText('email')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Email')
			->addRule($form::FILLED, 'Please enter your email')
			->addRule($form::EMAIL, 'Invalid email address');
        $form->addTextarea('message')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Message')
			->addRule($form::FILLED, 'Please enter your message');
		$form->addSubmit('send', 'Send');

		$form->onSuccess[] = [$this, 'processContactForm'];

		return $form;
    }
    
    /**
	 * Process contact form, send message
	 * @param Form
	 */
	public function processContactForm(Form $form)
	{
		$values = $form->getValues(true);

		$message = new Message;
		$message->addTo('give.me.r00m@vutbr.cz')
			->setFrom($values['email'])
			->setSubject('Message from contact form')
			->setBody($values['message'])
			->send();

		
		$this->redirect('Homepage:');
	}


}
